#include "Command.hpp"

void Command::partChannel(User &user, std::string &msg, std::string &ch_name) {
	if (!server_.hasChannelName(ch_name)) {
		std::cout << reply_.ERR_NOSUCHCHANNEL(ch_name) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NOSUCHCHANNEL(ch_name));
		return;
	} else if (!user.isMemberOfChannel(ch_name)) {
		std::cout << reply_.ERR_NOTONCHANNEL(ch_name) << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_NOTONCHANNEL(ch_name));

		return;
	}

	const Channel &left_ch = this->server_.getChannel(ch_name);
	if (left_ch.isChannelOperator(user.getFd())) {
		const_cast<Channel &>(left_ch).removeChannelOperator(user.getFd());
	}
	server_.sendToChannelUser(ch_name, ":" + user.getNickName() + "!" +
										   user.getUserName() + "ft_ircserver" +
										   " PART " + ch_name + " :" + msg);
	const_cast<Channel &>(left_ch).removeUser(user.getFd());
	if (left_ch.getJoinedUserCount() == 0) {
		this->server_.removeChannel(left_ch.getName());
	}
	user.removeChannel(ch_name);
}

void Command::PART(User &user, std::vector<std::string> &arg) {
	std::cout << "start part " << user.getFd() << std::endl;
	if (user.getAuthFlags() != User::ALL_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		server_.sendMsgToClient(user.getFd(), "client cannot authenticate");
		return;
	} else if (arg.size() < 2) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("PART") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NEEDMOREPARAMS("PART"));
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
			partChannel(user, msg, dsn.at(i));
		} else {
			std::cout << reply_.ERR_NOSUCHCHANNEL(dsn.at(i)) << std::endl;
			server_.sendMsgToClient(user.getFd(),
									reply_.ERR_NOSUCHCHANNEL(dsn.at(i)));
		}
	}
}
