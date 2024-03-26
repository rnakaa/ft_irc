#include "Command.hpp"

void Command::MODE(User &user, std::vector<std::string> &arg) {
	std::cout << "start MODE command" << std::endl;
	if (user.getAuthFlags() != User::ALL_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "client cannot authenticate");
		return;
	} else if (arg.empty() || arg.size() == 1) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("MODE") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NEEDMOREPARAMS("MODE"));
		return;
	}
	if (this->server_.hasChannelName(arg.at(0))) {
		handleChannelMode(user, arg, this->server_.getChannel(arg.at(0)));
	} else {
		// handleUserMode(user, arg);
		std::cerr << reply_.ERR_NOSUCHCHANNEL("MODE") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NOSUCHCHANNEL("MODE"));
		return;
	}
}

Command::ModeAction
Command::checkModeAction(const std::string &mode_str) const {
	if (mode_str[0] == '+') {
		return Command::setMode;
	} else if (mode_str[0] == '-') {
		return Command::unsetMode;
	} else {
		return Command::queryMode;
	}
}

void Command::joinStrFromVector(std::string &join_str, const Channel &ch,
								const std::string delimiter) {
	std::cout << "start joinStrFromVector" << std::endl;
	std::vector<std::string> vec = ch.getChannelOperatorsNickName();
	for (size_t i = 0; i < vec.size(); ++i) {
		std::cout << vec[i] << std::endl;
	}
	for (size_t i = 0; i < vec.size(); ++i) {
		join_str += vec.at(i);
		if (i < vec.size() - 1) {
			join_str += delimiter;
		}
	}
}

// mode "O"
void Command::handleChannelOriginOperator(const ModeAction mode_action,
										  User &user, const Channel &ch) {
	if (mode_action == Command::unsetMode || mode_action == Command::setMode) {
		std::cerr << reply_.ERR_NOPRIVILEGES() << std::endl;
		this->server_.sendMsgToClient(user.getFd(), reply_.ERR_NOPRIVILEGES());
		return;
	} else if (this->arg_.size() > 3) {
		std::cerr << "O: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "O: mode parameters are not required");
		return;
	}
	std::cout << ch.getName() << " " << ch.getCreatedUser() << std::endl;
	this->server_.sendMsgToClient(user.getFd(),
								  ch.getName() + " " + ch.getCreatedUser());
}

// mode "o": Give/take channel operator privilege
void Command::handleChannelOperator(const ModeAction mode_action, User &user,
									const Channel &ch) {
	if (mode_action == Command::queryMode) {
		if (this->arg_.size() > 2) {
			std::cerr << "o: mode parameters are not required" << std::endl;
			this->server_.sendMsgToClient(
				user.getFd(), "o: mode parameters are not required");
			return;
		}
		std::string send_str = ch.getName() + " ";
		joinStrFromVector(send_str, ch, ",");
		std::cout << send_str << std::endl;
		this->server_.sendMsgToClient(user.getFd(), send_str);
	} else {
		if (this->arg_.size() < 3) {
			std::cerr << reply_.ERR_NEEDMOREPARAMS("o") << std::endl;
			this->server_.sendMsgToClient(user.getFd(),
										  reply_.ERR_NEEDMOREPARAMS("o"));
			return;
		}
		for (size_t i = 2; i < this->arg_.size(); ++i) {
			setOrUnsetChannelOperator(i, mode_action, user, ch);
		}
	}
}

void Command::setOrUnsetChannelOperator(const size_t i,
										const ModeAction mode_action,
										User &user, const Channel &ch) {
	if (!this->server_.isUser(this->arg_.at(i))) {
		std::cerr << reply_.ERR_NOSUCHNICK(user.getNickName()) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_NOSUCHNICK(user.getNickName()));
		return;
	}
	const User &mode_user = this->server_.getUser(this->arg_.at(i));
	if (!ch.isChannelUser(mode_user.getFd())) {
		std::cerr << reply_.ERR_USERNOTINCHANNEL(mode_user.getNickName(),
												 ch.getName())
				  << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(),
			reply_.ERR_USERNOTINCHANNEL(mode_user.getNickName(), ch.getName()));
		return;
	} else if (mode_action == Command::setMode) {
		if (ch.isChannelOperator(mode_user.getFd())) {
			std::cerr << mode_user.getNickName() << " is already channel "
					  << ch.getName() << " operator" << std::endl;
			this->server_.sendMsgToClient(
				user.getFd(), mode_user.getNickName() + " is already channel " +
								  ch.getName() + " operator");
			return;
		}
		const_cast<Channel &>(ch).setChannelOperator(mode_user.getFd());
		std::cout << ch.getName() << " " << mode_user.getNickName()
				  << " is now channel operator" << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), ch.getName() + " " + mode_user.getNickName() +
							  " is now channel operator");
	} else if (mode_action == Command::unsetMode) {
		if (mode_user.getNickName() == ch.getCreatedUser()) {
			std::cerr << "cannot unset channel operator because "
					  << mode_user.getNickName() << " is channel creator";
			this->server_.sendMsgToClient(
				user.getFd(), "cannot unset channel operator because " +
								  mode_user.getNickName() +
								  " is channel creator");
			return;
		} else if (!ch.isChannelOperator(mode_user.getFd())) {
			std::cerr << mode_user.getNickName() << " is already not channel "
					  << ch.getName() << " operator" << std::endl;
			this->server_.sendMsgToClient(user.getFd(),
										  mode_user.getNickName() +
											  " is already not channel " +
											  ch.getName() + " operator");
			return;
		}
		const_cast<Channel &>(ch).removeChannelOperator(mode_user.getFd());
		std::cout << ch.getName() << " " << mode_user.getNickName()
				  << " is now not channel operator" << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), ch.getName() + " " + mode_user.getNickName() +
							  " is now not channel operator");
	}
}

bool Command::checkInvalidSignsCount(const std::string &mode_str) {
	int count = 0;
	for (size_t i = 0; i < mode_str.size(); ++i) {
		if (mode_str[i] == '+' || mode_str[i] == '-') {
			count++;
		}
		if (count > 1) {
			return false;
		}
	}
	return true;
}

void Command::handleChannelMode(User &user, std::vector<std::string> &arg,
								const Channel &ch) {
	size_t i = 0;
	std::string mode_str = arg.at(1);
	if (!checkInvalidSignsCount(mode_str)) {
		std::cerr << reply_.ERR_UMODEUNKNOWNFLAG(mode_str) << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_UMODEUNKNOWNFLAG(mode_str));
		return;
	}
	ModeAction mode_action = checkModeAction(mode_str);
	if ((mode_action == Command::setMode ||
		 mode_action == Command::unsetMode) &&
		!ch.isChannelOperator(user.getFd())) {
		std::cerr << reply_.ERR_CHANOPRIVSNEEDED(ch.getName()) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_CHANOPRIVSNEEDED(ch.getName()));
		return;
	}
	if (mode_action == Command::setMode || mode_action == Command::unsetMode) {
		i++;
	}
	std::cout << "start handleChannelMode: " << mode_action << std::endl;
	for (; i < mode_str.size(); ++i) {
		char mode_type = mode_str[i];
		ModeFunction mode_func = this->mode_map_[mode_type];
		if (!mode_func) {
			std::cerr << reply_.ERR_UNKNOWNMODE(std::string(1, mode_type),
												ch.getName())
					  << std::endl;
			this->server_.sendMsgToClient(
				user.getFd(), reply_.ERR_UNKNOWNMODE(std::string(1, mode_type),
													 ch.getName()));
			continue;
		}
		std::cout << "mode_type: " << mode_type << std::endl;
		(this->*mode_func)(mode_action, user, ch);
	}
}
