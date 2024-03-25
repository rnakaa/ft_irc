#include "Command.hpp"

void Command::MODE(User &user, std::vector<std::string> &arg) {
	std::cout << "start MODE command" << std::endl;
	if (user.getAuthFlags() != User::ALL_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "client cannot authenticate");
		return;
	} else if (arg.empty() || arg.size() == 1) {
		std::cerr << error_.ERR_NEEDMOREPARAMS("MODE") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  error_.ERR_NEEDMOREPARAMS("MODE"));
		return;
	}
	if (this->server_.hasChannelName(arg.at(0))) {
		handleChannelMode(user, arg, this->server_.getChannel(arg.at(0)));
	} else {
		// handleUserMode(user, arg);
		std::cerr << error_.ERR_NOSUCHCHANNEL("MODE") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  error_.ERR_NOSUCHCHANNEL("MODE"));
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

void Command::handleChannelOriginOperator(const ModeAction mode_action,
										  User &user, const Channel &ch) {
	if (mode_action == Command::unsetMode || mode_action == Command::setMode) {
		std::cerr << error_.ERR_NOPRIVILEGES() << std::endl;
		this->server_.sendMsgToClient(user.getFd(), error_.ERR_NOPRIVILEGES());
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
		std::cerr << error_.ERR_UMODEUNKNOWNFLAG(mode_str) << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  error_.ERR_UMODEUNKNOWNFLAG(mode_str));
		return;
	}
	ModeAction mode_action = checkModeAction(mode_str);
	if ((mode_action == Command::setMode ||
		 mode_action == Command::unsetMode) &&
		!ch.isChannelOperator(user)) {
		std::cerr << error_.ERR_CHANOPRIVSNEEDED(ch.getName()) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), error_.ERR_CHANOPRIVSNEEDED(ch.getName()));
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
			std::cerr << error_.ERR_UNKNOWNMODE(std::string(1, mode_type),
												ch.getName())
					  << std::endl;
			this->server_.sendMsgToClient(
				user.getFd(), error_.ERR_UNKNOWNMODE(std::string(1, mode_type),
													 ch.getName()));
			continue;
		}
		std::cout << "mode_type: " << mode_type << std::endl;
		(this->*mode_func)(mode_action, user, ch);
	}
}
