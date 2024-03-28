#include "Command.hpp"

void Command::quitAllChannels(User &user, std::string broadcast_msg) {
	if (user.getJoinedChannelCount() == 0)
		return;
	const std::set<std::string> joined_ch = user.getJoinedChannels();
	for (std::set<std::string>::iterator it = joined_ch.begin();
		 it != joined_ch.end(); ++it) {
		const std::string ch_name = *it;
		const Channel &left_ch_const = this->server_.getChannel(ch_name);
		if (left_ch_const.isChannelOperator(user.getFd())) {
			const_cast<Channel &>(left_ch_const)
				.removeChannelOperator(user.getFd());
		}
		// remove from all invite lists
		// send broadcast_msg to all channel members
		(void)broadcast_msg;
		const_cast<Channel &>(left_ch_const).removeUser(user.getFd());
		if (left_ch_const.getJoinedUserCount() == 0) {
			this->server_.removeChannel(left_ch_const.getName());
		}
	}
}

void Command::QUIT(User &user, std::vector<std::string> &arg) {
	// close?
	std::string broadcast_msg;

	if (user.getAuthFlags() != User::ALL_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		server_.sendMsgToClient(user.getFd(), "client cannot authenticate");
		return;
	} else if (arg.empty()) {
		broadcast_msg = " QUIT";
	} else if (arg.at(0)[0] != ':') {
		broadcast_msg = " QUIT :" + arg.at(0);
	} else {
		broadcast_msg = " QUIT :" + extractAfterColon(1);
		std::cout << "broadcast_msg" << broadcast_msg << std::endl; // debug
	}
	quitAllChannels(user, broadcast_msg);
	// send error message to client?
	const int fd = user.getFd();
	this->server_.removeUser(fd);
	this->server_.removePollfd(fd);
	close(fd);
}
