#include "Command.hpp"

std::vector<std::string> Command::splitByComma(const std::string &input) {
	std::vector<std::string> result;
	std::istringstream iss(input);
	std::string token;

	while (std::getline(iss, token, ',')) {
		result.push_back(token);
	}
	return result;
}

void Command::sendMessage(User &sender, const std::string &dsn,
						  const std::string &msg) {
	if (!server_.isUser(dsn))
		return;
	User usr = server_.getUser(dsn);
	server_.sendMsgToClient(usr.getFd(), ":" + sender.getNickName() + "!" +
											 sender.getUserName() +
											 "ft_ircserver" + " PRIVMSG " +
											 usr.getNickName() + " :" + msg);
}

void Command::PRIVMSG(User &user, std::vector<std::string> &arg) {
	std::cout << "start PRIVMSG command" << std::endl;
	if (user.getAuthFlags() != User::ALL_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		server_.sendMsgToClient(user.getFd(), "client cannot authenticate");
		return;
	} else if (arg.size() < 2) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("PRIVMSG") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NEEDMOREPARAMS("PRIVMSG"));
		return;
	}

	std::string arg1 = arg.at(0);
	std::string msg;
	std::vector<std::string> dsn = splitByComma(arg1);
	for (size_t i = 1; i < arg.size(); i++) {
		msg += arg.at(i) + " ";
	}

	if (msg[0] == ':') {
		msg.substr(1); // メッセージの先頭に:がついていたら削除する
	}

	for (size_t i = 0; i < dsn.size(); i++) {
		if (dsn.at(i)[0] == '!' || dsn.at(i)[0] == '+' || dsn.at(i)[0] == '&' ||
			dsn.at(i)[0] == '#') {
			server_.sendToChannelUser(dsn.at(i), user, msg);
		} else {
			sendMessage(user, dsn.at(i), msg);
		}
	}
}
