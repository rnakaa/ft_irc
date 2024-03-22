#include "Command.hpp"

bool Command::startWithChannelChar(const std::string &str) {
	return !str.empty() &&
		   (str[0] == '&' || str[0] == '#' || str[0] == '+' || str[0] == '!');
}

bool Command::setArgToVec(const std::vector<std::string> &arg,
						  std::queue<std::string> &ch_queue,
						  std::queue<std::string> &key_queue) {
	bool is_push_key = false;
	for (size_t i = 0; i < arg.size(); ++i) {
		if (startWithChannelChar(arg.at(i))) {
			if (is_push_key) {
				return false;
			}
			ch_queue.push(arg.at(i));
		} else {
			key_queue.push(arg.at(i));
			is_push_key = true;
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
	} else if (ch_name.find(' ') != std::string::npos || ch_name.find('\a') != std::string::npos || ch_name.find(',') != std::string::npos) {
		return false;
	}
	return true;
}

void Command::joinChannel(const std::string &ch_name, const std::string &ch_key, User &user) {
	Channel join_ch = this->server_.getChannel(ch_name);
	if (user.isMemberOfChannel(join_ch.getName())) {
		std::cerr << "client already join channel received message"
				  << std::endl;
		server_.sendMsgToClient(user.getFd(),
								"client already join channel received message");
		return;
	} else if (join_ch.getPass() != ch_key) {
		std::cerr << "channel password is not correct"
				  << std::endl;
		server_.sendMsgToClient(user.getFd(),
								"channel password is not correct");
		return;
	}
	user.setChannel(join_ch.getName(), join_ch);
	std::cout << "finish JOIN command" << std::endl;
	user.printJoinChannel();
	this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN Command");
}

void Command::createChannel(const std::string &ch_name, const std::string &ch_key, User &user) {
	Channel new_ch(ch_name, ch_key, user);
	this->server_.setChannel(new_ch.getName(), new_ch);
	user.setChannel(new_ch.getName(), new_ch);
	std::cout << "finish JOIN command" << std::endl;
	user.printJoinChannel();
	this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN Command");
}

void Command::handleChannelRequests(std::queue<std::string> &ch_queue, std::queue<std::string> &key_queue, User &user) {
	std::string ch_name(ch_queue.front()), ch_key;
	ch_queue.pop();
	if (key_queue.size() > 0) {
		ch_key = key_queue.front();
		key_queue.pop();
	} else {
		ch_key = "";
	}
	if (!checkValidChannel(ch_name)) {
		std::cerr << "invalid channel name: " << ch_name << std::endl;
		server_.sendMsgToClient(user.getFd(), "invalid channel name: " + ch_name);
		return ;
	}
	if (this->server_.hasChannelName(ch_name)) {
		joinChannel(ch_name, ch_key, user);
	} else {
		createChannel(ch_name, ch_key, user);
	}
}

void Command::JOIN(User &user, std::vector<std::string> &arg) {
	std::cout << "start JOIN command" << std::endl;
	// if (user.getAuthFlags() != User::ALL_AUTH) {
	if (user.getAuthFlags() != User::PASS_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		server_.sendMsgToClient(user.getFd(), "client cannot authenticate");
		return;
	} else if (arg.empty()) {
		std::cerr << error_.ERR_NEEDMOREPARAMS("JOIN") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_NEEDMOREPARAMS("JOIN"));
		return;
	}
	std::queue<std::string> ch_queue, key_queue;
	if (!setArgToVec(arg, ch_queue, key_queue)) {
		std::cerr << "incorrect channel and key order" << std::endl;
		server_.sendMsgToClient(user.getFd(), "incorrect channel and key order");
		return;
	}
	if (!checkValidArg(ch_queue, key_queue)) {
		std::cerr << "invalid arg" << std::endl;
		server_.sendMsgToClient(user.getFd(), "invalid arg");
		return;
	}
	while (ch_queue.size() > 0) {
		handleChannelRequests(ch_queue, key_queue, user);
	}
}
