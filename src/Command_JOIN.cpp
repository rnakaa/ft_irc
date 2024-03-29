#include "Command.hpp"

bool Command::startWithChannelChar(const std::string &str) {
	return !str.empty() &&
		   (str[0] == '&' || str[0] == '#' || str[0] == '+' || str[0] == '!');
}

bool Command::setArgToVec(const std::vector<std::string> &arg,
						  std::queue<std::string> &ch_queue,
						  std::queue<std::string> &key_queue) {
	std::istringstream ch_iss, key_iss;
	std::string ch_token, key_token, rest;
	bool is_key = false;
	if (arg.size() > 2) {
		return false;
	} else if (arg.size() == 2) {
		ch_iss.str(arg.at(0));
		key_iss.str(arg.at(1));
		is_key = true;
	} else {
		ch_iss.str(arg.at(0));
	}

	while (std::getline(ch_iss, ch_token, ',')) {
		if (!startWithChannelChar(ch_token)) {
			std::cout << "false" << std::endl;
			return false;
		}
		ch_queue.push(ch_token);
	}
	if (is_key) {
		while (std::getline(key_iss, key_token, ',')) {
			if (startWithChannelChar(key_token)) {
				return false;
			}
			key_queue.push(key_token);
		}
	}
	return true;
}

bool Command::checkValidArg(const std::queue<std::string> &ch_queue,
							const std::queue<std::string> &key_queue) {
	return ch_queue.size() != 0 && ch_queue.size() >= key_queue.size();
}

bool Command::checkValidChannel(const std::string &ch_name) {
	if (ch_name.size() > 50 || ch_name.size() == 1) {
		return false;
	} else if (ch_name.find(' ') != std::string::npos ||
			   ch_name.find('\a') != std::string::npos ||
			   ch_name.find(',') != std::string::npos) {
		return false;
	}
	return true;
}

void Command::joinChannel(const std::string &ch_name, const std::string &ch_key,
						  User &user) {
	const Channel &join_ch_const = this->server_.getChannel(ch_name);
	join_ch_const.printJoinedUser();
	Channel &join_ch = const_cast<Channel &>(join_ch_const);
	if (user.isMemberOfChannel(join_ch.getName())) {
		std::cerr << "client already join channel received from message"
				  << std::endl;
		server_.sendMsgToClient(
			user.getFd(), "client already join channel received from message");
		return;
	} else if (join_ch.hasMode(Channel::i) &&
			   !join_ch.isInvitedUser(user.getFd())) {

		std::cerr << reply_.ERR_INVITEONLYCHAN(ch_name) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_INVITEONLYCHAN(ch_name));
		return;
	} else if (static_cast<ssize_t>(join_ch.getJoinedUserCount()) ==
			   join_ch.getMaxUsers()) {
		std::cerr << reply_.ERR_CHANNELISFULL(ch_name) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_CHANNELISFULL(ch_name));
		return;
	} else if (join_ch.getPass() != ch_key) {
		std::cerr << reply_.ERR_BADCHANNELKEY(ch_name) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_BADCHANNELKEY(ch_name));
		return;
	}
	user.setChannel(join_ch);
	join_ch.setUser(user);
	std::cout << "finish JOIN command" << std::endl;
	user.printJoinChannel();
	join_ch.printJoinedUser();
	this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN Command");
}

void Command::createChannel(const std::string &ch_name,
							const std::string &ch_key, User &user) {
	Channel new_ch(ch_name, ch_key, user);
	new_ch.setMode(Channel::O);
	new_ch.setChannelOperator(user.getFd());
	this->server_.setChannel(new_ch.getName(), new_ch);
	user.setChannel(new_ch);
	std::cout << "finish JOIN command" << std::endl;
	user.printJoinChannel();
	const Channel ch = this->server_.getChannel(ch_name);
	ch.printJoinedUser();
	ch.printChannelOperators();
	this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN Command");
}

void Command::handleChannelRequests(std::queue<std::string> &ch_queue,
									std::queue<std::string> &key_queue,
									User &user) {
	std::string ch_name(ch_queue.front()), ch_key;
	ch_queue.pop();
	if (key_queue.size() > 0) {
		ch_key = key_queue.front();
		key_queue.pop();
	} else {
		ch_key = "";
	}
	if (!checkValidChannel(ch_name)) {
		std::cerr << reply_.ERR_NOSUCHCHANNEL(ch_name) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NOSUCHCHANNEL(ch_name));
		return;
	}
	if (this->server_.hasChannelName(ch_name)) {
		joinChannel(ch_name, ch_key, user);
	} else {
		createChannel(ch_name, ch_key, user);
	}
}

void Command::exitAllChannels(User &user) {
	const std::set<std::string> joined_ch = user.getJoinedChannels();
	for (std::set<std::string>::iterator it = joined_ch.begin();
		 it != joined_ch.end(); ++it) {
		const std::string ch_name = *it;
		const Channel &left_ch_const = this->server_.getChannel(ch_name);
		if (left_ch_const.isChannelOperator(user.getFd())) {
			const_cast<Channel &>(left_ch_const)
				.removeChannelOperator(user.getFd());
		}
		const_cast<Channel &>(left_ch_const).removeUser(user.getFd());
		if (left_ch_const.getJoinedUserCount() == 0) {
			this->server_.removeChannel(left_ch_const.getName());
		}
		user.removeChannel(ch_name);
	}
}

void Command::JOIN(User &user, std::vector<std::string> &arg) {
	std::cout << "start JOIN command" << std::endl;
	if (user.getAuthFlags() != User::ALL_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		server_.sendMsgToClient(user.getFd(), "client cannot authenticate");
		return;
	} else if (arg.empty()) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("JOIN") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NEEDMOREPARAMS("JOIN"));
		return;
	}
	if (arg.at(0) == "0") {
		exitAllChannels(user);
		std::cout << "finish JOIN 0 command" << std::endl;
		user.printJoinChannel();
		this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN 0 Command");
		return;
	}
	std::queue<std::string> ch_queue, key_queue;
	if (!setArgToVec(arg, ch_queue, key_queue)) {
		std::cerr << "incorrect channel and key arg" << std::endl;
		server_.sendMsgToClient(user.getFd(), "incorrect channel and key arg");
		return;
	}
	if (!checkValidArg(ch_queue, key_queue)) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("JOIN") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NEEDMOREPARAMS("JOIN"));
		return;
	}
	while (ch_queue.size() > 0) {
		handleChannelRequests(ch_queue, key_queue, user);
	}
}
