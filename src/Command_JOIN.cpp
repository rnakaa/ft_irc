#include "Command.hpp"

bool Command::startWithChannelChar(const std::string &str) {
	return !str.empty() &&
		   (str[0] == '&' || str[0] == '#' || str[0] == '+' || str[0] == '!');
}

void Command::setArgToVec(const std::vector<std::string> &arg,
						  std::vector<std::string> &ch_vec,
						  std::vector<std::string> &key_vec) {
	for (size_t i = 0; i < arg.size(); ++i) {
		if (startWithChannelChar(arg.at(i))) {
			ch_vec.push_back(arg.at(i));
			// std::cout << "ch_vec: " << arg.at(i) << std::endl;
		} else {
			key_vec.push_back(arg.at(i));
			// std::cout << "key_vec: " << arg.at(i) << std::endl;
		}
	}
}

void Command::joinChannel(const std::string &ch_name, User &user) {
	Channel join_ch = this->server_.getChannel(ch_name);
	if (user.isMemberOfChannel(join_ch.getName())) {
		std::cerr << "client already join channel received message"
				  << std::endl;
		server_.sendMsgToClient(user.getFd(),
								"client already join channel received message");
		return;
	}
	user.setChannel(join_ch.getName(), join_ch);
	std::cout << "finish JOIN command" << std::endl;
	user.printJoinChannel();
	this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN Command");
}

void Command::createChannel(const std::string &ch_name, User &user) {
	Channel new_ch(ch_name, "", user);
	this->server_.setChannel(new_ch.getName(), new_ch);
	user.setChannel(new_ch.getName(), new_ch);
	std::cout << "finish JOIN command" << std::endl;
	user.printJoinChannel();
	this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN Command");
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
	std::vector<std::string> ch_vec, key_vec;
	setArgToVec(arg, ch_vec, key_vec);
	std::cout << "ch_vec: ";
	for (size_t i = 0; i < ch_vec.size(); ++i) {
		std::cout << ch_vec.at(i) << ", ";
	}
	std::cout << std::endl;
	std::cout << "key_vec: ";
	for (size_t i = 0; i < key_vec.size(); ++i) {
		std::cout << key_vec.at(i) << ", ";
	}
	std::cout << std::endl;
	// if (checkValidArg(ch_vec, key_vec)) {
	// 	std::cerr << "invalid channel name" << std::endl;
	// 	server_.sendMsgToClient(user.getFd(), "invalid channel name");
	// 	return ;
	// }
	// // if (ch_vec.size() == 0 || ch_vec.size() < key_vec.size()) {
	// // 	std::cerr << error_.ERR_NEEDMOREPARAMS("JOIN") << std::endl;
	// // 	server_.sendMsgToClient(user.getFd(),
	// // 							error_.ERR_NEEDMOREPARAMS("JOIN"));
	// // 	return ;
	// // }
	// for (size_t i = 0; i < ch_vec.size(); ++i) {
	for (size_t i = 0; i < arg.size(); ++i) {
		if (this->server_.hasChannelName(arg.at(i))) {
			// joinChannel(i, ch_vec, key_vec);
			joinChannel(arg.at(i), user);
			// joinChannel(i, ch_vec, key_vec);
		} else {
			// createChannel(i, ch_vec, key_vec);
			createChannel(arg.at(i), user);
		}
	}
	// std::cout << "finish JOIN command" << std::endl;
	// user.printJoinChannel();
	// this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN Command");
}
