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
		this->server_.sendToChannelUser(
			ch.getName(), ":" + user.getNickName() + "!" + user.getUserName() +
							  "ft_ircserver" + " MODE +o " + ch.getName() +
							  " :" + mode_user.getNickName() +
							  " is now channel operator");
		// this->server_.sendMsgToClient(
		// 	user.getFd(), ch.getName() + " " + mode_user.getNickName() +
		// 					  " is now channel operator");
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
		// this->server_.sendMsgToClient(
		// 	user.getFd(), ch.getName() + " " + mode_user.getNickName() +
		// 					  " is now not channel operator");
		this->server_.sendToChannelUser(
			ch.getName(), ch.getName() + " " + mode_user.getNickName() +
							  " is now not channel operator");
	}
}

// mode "k": Set/remove the channel key (password)
void Command::handleChannelKey(const ModeAction mode_action, User &user,
							   const Channel &ch) {
	if (!ch.isChannelOperator(user.getFd())) {
		std::cerr << reply_.ERR_CHANOPRIVSNEEDED(ch.getName()) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_CHANOPRIVSNEEDED(ch.getName()));
		return;
	}
	if (mode_action == Command::queryMode) {
		handleKeyQueryMode(user, ch);
	} else if (mode_action == Command::setMode) {
		handleKeySetMode(user, ch);
	} else {
		handleKeyUnsetMode(user, ch);
	}
}

void Command::handleKeyQueryMode(User &user, const Channel &ch) {
	if (this->arg_.size() > 2) {
		std::cerr << "k: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "k: mode parameters are not required");
		return;
	}
	std::cout << ch.getName() << " key: " << ch.getPass() << std::endl;
	this->server_.sendMsgToClient(user.getFd(),
								  ch.getName() + " key: " + ch.getPass());
}

void Command::handleKeySetMode(User &user, const Channel &ch) {
	if (this->arg_.size() < 3) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("MODE k flag") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NEEDMOREPARAMS("MODE k flag"));
		return;
	} else if (this->arg_.size() > 3) {
		std::cerr << "k: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "k: mode parameters are not required");
		return;
	}
	const_cast<Channel &>(ch).setPass(this->arg_.at(2));
	std::cout << ch.getName() << " key is now " << ch.getPass() << std::endl;
	this->server_.sendMsgToClient(user.getFd(),
								  ch.getName() + " key is now " + ch.getPass());
}

void Command::handleKeyUnsetMode(User &user, const Channel &ch) {
	if (this->arg_.size() < 3) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("MODE k flag") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NEEDMOREPARAMS("MODE k flag"));
		return;
	} else if (this->arg_.size() > 3) {
		std::cerr << "k: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "k: mode parameters are not required");
		return;
	}
	if (ch.getPass() == "") {
		std::cerr << ch.getName() << " is already not set password"
				  << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), ch.getName() + " is already not set password");
	} else if (this->arg_.at(2) != ch.getPass()) {
		std::cerr << reply_.ERR_PASSWDMISMATCH() << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_PASSWDMISMATCH());
	} else {
		const_cast<Channel &>(ch).setPass("");
		std::cout << ch.getName() << " key is now unset" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  ch.getName() + " key is now unset");
	}
}

// mode "l": Set/remove the user limit to channel
void Command::handleLimitedUserNum(const ModeAction mode_action, User &user,
								   const Channel &ch) {
	if (mode_action == Command::queryMode) {
		handleLimitedQueryMode(user, ch);
		return;
	}
	if (!ch.isChannelOperator(user.getFd())) {
		std::cerr << reply_.ERR_CHANOPRIVSNEEDED(ch.getName()) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_CHANOPRIVSNEEDED(ch.getName()));
		return;
	} else if (mode_action == Command::setMode) {
		handleLimitedSetMode(user, ch);
	} else {
		handleLimitedUnsetMode(user, ch);
	}
}

void Command::handleLimitedQueryMode(User &user, const Channel &ch) {
	if (this->arg_.size() > 2) {
		std::cerr << "l: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "l: mode parameters are not required");
		return;
	}
	std::ostringstream oss_joined_user_num;
	oss_joined_user_num << ch.getJoinedUserCount();
	std::cout << "getMaxUsers(): " << ch.getMaxUsers() << std::endl;
	if (ch.getMaxUsers() == -1) {
		std::cout << ch.getName() << " has not a limit of users" << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), ch.getName() + " has not a limit of users");
		return;
	}
	std::ostringstream oss_max_user_num;
	oss_max_user_num << ch.getMaxUsers();
	std::cout << ch.getName() << " has a limit of " << oss_max_user_num.str()
			  << " users" << std::endl;
	this->server_.sendMsgToClient(user.getFd(),
								  ch.getName() + " has a limit of " +
									  oss_max_user_num.str() + " users");
	return;
}

void Command::handleLimitedSetMode(User &user, const Channel &ch) {
	if (this->arg_.size() < 3) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("MODE l flag") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NEEDMOREPARAMS("MODE l flag"));
		return;
	} else if (this->arg_.size() > 3) {
		std::cerr << "+l: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "+l: mode parameters are not required");
		return;
	}
	std::istringstream iss(this->arg_.at(2));
	long max_users;
	iss >> max_users;
	if (std::numeric_limits<int>::max() < max_users) {
		std::cerr << "too large user limits" << std::endl;
		this->server_.sendMsgToClient(user.getFd(), "too large user limits");
		return;
	} else if (max_users < static_cast<long>(ch.getJoinedUserCount())) {
		std::cerr << "smaller than current joined users" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "smaller than current joined users");
		return;
	}
	std::cout << "max_users: " << max_users
			  << " joineduser: " << ch.getJoinedUserCount() << std::endl;
	const_cast<Channel &>(ch).setMaxUsers(static_cast<int>(max_users));
	std::cout << ch.getName() << " set max number of users to "
			  << ch.getMaxUsers() << std::endl;
	// this->server_.sendMsgToClient(
	// 	user.getFd(),
	// 	ch.getName() + " set max number of users to " + iss.str());
	this->server_.sendToChannelUser(
		ch.getName(),
		ch.getName() + " set max number of users to " + iss.str());
}

void Command::handleLimitedUnsetMode(User &user, const Channel &ch) {
	if (this->arg_.size() > 2) {
		std::cerr << "-l: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "-l: mode parameters are not required");
		return;
	} else if (ch.getMaxUsers() == -1) {
		std::cerr << ch.getName() << " is already unset max number of users"
				  << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(),
			ch.getName() + " is already unset max number of users");
		return;
	}
	const_cast<Channel &>(ch).setMaxUsers(-1);
	std::cout << ch.getName() << " remove a limit of the max users"
			  << std::endl;
	// this->server_.sendMsgToClient(
	// 	user.getFd(), ch.getName() + " remove a limit of the max users");
	this->server_.sendToChannelUser(
		ch.getName(), ch.getName() + " remove a limit of the max users");
}

// mode "i":set/remove Invite-only channel
void Command::handleInviteOnly(const ModeAction mode_action, User &user,
							   const Channel &ch) {
	if (this->arg_.size() > 2) {
		std::cerr << "i: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "i: mode parameters are not required");
		return;
	}
	if (mode_action == Command::queryMode) {
		queryInviteOnly(user, ch);
		return;
	}
	if (!ch.isChannelOperator(user.getFd())) {
		std::cerr << reply_.ERR_CHANOPRIVSNEEDED(ch.getName()) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_CHANOPRIVSNEEDED(ch.getName()));
		return;
	} else if (mode_action == Command::setMode) {
		setInviteOnly(user, ch);
	} else {
		unsetInviteOnly(user, ch);
	}
}

void Command::queryInviteOnly(User &user, const Channel &ch) {
	std::vector<std::string> invited_user = ch.getInvitedUsersNickName();
	if (invited_user.empty()) {
		std::cerr << "no invited users" << std::endl;
		this->server_.sendMsgToClient(user.getFd(), "no invited users");
		return;
	}
	std::ostringstream oss;
	oss << ch.getName() << " invited users: ";
	for (size_t i = 0; i < invited_user.size(); ++i) {
		oss << invited_user.at(i);
		if (i < invited_user.size() - 1) {
			oss << ", ";
		}
	}
	std::cout << oss.str() << std::endl;
	this->server_.sendMsgToClient(user.getFd(), oss.str());
}

void Command::setInviteOnly(User &user, const Channel &ch) {
	if (ch.hasMode(Channel::i)) {
		std::cerr << ch.getName() << " is already set i mode" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  ch.getName() + " is already set i mode");
		return;
	}
	const_cast<Channel &>(ch).setMode(Channel::i);
	std::cout << ch.getName() << " is now set i mode" << std::endl;
	// this->server_.sendMsgToClient(user.getFd(),
	// 							  ch.getName() + " is now set i mode");
	this->server_.sendToChannelUser(ch.getName(),
									ch.getName() + " is now set i mode");
}

void Command::unsetInviteOnly(User &user, const Channel &ch) {
	if (!ch.hasMode(Channel::i)) {
		std::cerr << ch.getName() << " is already unset i mode" << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), ch.getName() + " is already unset i mode");
		return;
	}
	const_cast<Channel &>(ch).unsetMode(Channel::i);
	std::cout << ch.getName() << " is now unset i mode" << std::endl;
	// this->server_.sendMsgToClient(user.getFd(),
	// 							  ch.getName() + " is now unset i mode");
	this->server_.sendToChannelUser(ch.getName(),
									ch.getName() + " is now unset i mode");
}

// mode "t":Set/remove the restrictions of the TOPIC command to channel
// operators
void Command::handleTopicOnlyOperator(const ModeAction mode_action, User &user,
									  const Channel &ch) {
	if (!ch.isChannelOperator(user.getFd())) {
		std::cerr << reply_.ERR_CHANOPRIVSNEEDED(ch.getName()) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_CHANOPRIVSNEEDED(ch.getName()));
		return;
	} else if (this->arg_.size() > 2) {
		std::cerr << reply_.ERR_TOOMANYPARAMS("Mode t flag") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_TOOMANYPARAMS("Mode t flag"));
		return;
	} else if (this->arg_.size() < 2) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("Mode t flag") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NEEDMOREPARAMS("Mode t flag"));
		return;
	} else if (mode_action == Command::queryMode) {
		std::cerr << reply_.ERR_UMODEUNKNOWNFLAG("t") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_UMODEUNKNOWNFLAG("t"));
		return;
	}
	if (mode_action == Command::setMode) {
		if (ch.hasMode(Channel::t)) {
			std::cerr << ch.getName() << " is already set t mode" << std::endl;
			this->server_.sendMsgToClient(
				user.getFd(), ch.getName() + " is already set t mode");
			return;
		}
		const_cast<Channel &>(ch).setMode(Channel::t);
		std::cout << ch.getName() << " is now set t mode" << std::endl;
		// this->server_.sendMsgToClient(user.getFd(),
		// 							  ch.getName() + " is now set t mode");
		this->server_.sendToChannelUser(ch.getName(),
										ch.getName() + " is now set t mode");
	} else {
		if (!ch.hasMode(Channel::t)) {
			std::cerr << ch.getName() << " is already not set t mode"
					  << std::endl;
			this->server_.sendMsgToClient(
				user.getFd(), ch.getName() + " is already not set t mode");
			return;
		}
		const_cast<Channel &>(ch).unsetMode(Channel::t);
		std::cout << ch.getName() << " is now unset t mode" << std::endl;
		// this->server_.sendMsgToClient(user.getFd(),
		// 							  ch.getName() + " is now unset t mode");
		this->server_.sendToChannelUser(ch.getName(),
										ch.getName() + " is now unset t mode");
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
