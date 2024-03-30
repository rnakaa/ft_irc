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
		if (left_ch_const.isChannelCreater(user.getFd())) {
			const_cast<Channel &>(left_ch_const).setChannelCreater(-1);
		}
		// remove from all invite lists
		// send broadcast_msg to all channel members
		this->server_.sendToChannelAllUser(left_ch_const.getName(), user,
										   broadcast_msg);
		const_cast<Channel &>(left_ch_const).removeUser(user.getFd());
		if (left_ch_const.getJoinedUserCount() == 0) {
			this->server_.removeChannel(left_ch_const.getName());
		}
	}
}

void Command::removeAllInvitedChannels(User &user) {
	if (user.getInvitedChannelCount() == 0) {
		return;
	}
	const std::vector<std::string> &invited_ch = user.getInvitedChannel();
	for (std::vector<std::string>::const_iterator it = invited_ch.begin();
		 it != invited_ch.end(); ++it) {
		const std::string &ch_name = *it;
		const Channel &ch = this->server_.getChannel(ch_name);
		const_cast<Channel &>(ch).removeInvitedUser(user.getFd());
	}
}

void Command::QUIT(User &user, std::vector<std::string> &arg) {
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
	}
	removeAllInvitedChannels(user);
	quitAllChannels(user, broadcast_msg);
	const int fd = user.getFd();
	this->server_.removeUser(fd);
	this->server_.removePollfd(fd);
	close(fd);
}
