#include "Command.hpp"

// void Command::setArgToVec(const std::vector<std::string> &arg,
// std::vector<std::string> &ch_vec, std::vector<std::string> &key_vec {
//
// }

bool Command::isChannelName(const std::string &ch_name) {
	return this->server_.hasChannelName(ch_name);
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
	// for (size_t i = 0; i < arg.size(); ++i) {
	// 	Channel ch(arg.at(i), "", user);
	// 	this->server_.setChannel(ch.getName(), ch);
	// 	user.setChannel(ch.getName(), ch);
	// }
	// std::vector<std::string> ch_vec, key_vec;
	// setArgToVec(arg, ch_vec, key_vec);
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
		if (isChannelName(arg.at(i))) {
			std::cout << "TRUE isChannelName()" << std::endl;
			// joinChannel(i, ch_vec, key_vec);
		} else {
			std::cout << "FALSE isChannelName()" << std::endl;
			// createChannel(i, ch_vec, key_vec);
			Channel ch(arg.at(i), "", user);
			this->server_.setChannel(ch.getName(), ch);
			user.setChannel(ch.getName(), ch);
		}
	}
	std::cout << "finish JOIN command" << std::endl;
	this->server_.sendMsgToClient(user.getFd(), "SUCCESS: JOIN Command");
}
