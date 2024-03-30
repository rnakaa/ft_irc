#include <Command.hpp>

void Command::KICK(User &user, std::vector<std::string> &arg) {
	if (user.getAuthFlags() != User::ALL_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  "client cannot authenticate");
		return;
	} else if (arg.empty() || arg.size() < 2) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("KICK") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NEEDMOREPARAMS("KICK"));
		return;
	} else if (!this->server_.hasChannelName(this->arg_.at(0))) {
		std::cerr << reply_.ERR_NOSUCHCHANNEL(this->arg_.at(0)) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_NOSUCHCHANNEL(this->arg_.at(0)));
		return;
	}
	const Channel &ch = this->server_.getChannel(arg.at(0));
	if (!ch.isChannelUser(user.getFd())) {
		std::cerr << reply_.ERR_NOTONCHANNEL(arg.at(0)) << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NOTONCHANNEL(arg.at(0)));
		return;
	} else if (!ch.isChannelOperator(user.getFd())) {
		std::cerr << reply_.ERR_CHANOPRIVSNEEDED(ch.getName()) << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_CHANOPRIVSNEEDED(ch.getName()));
		return;
	}
	if (!this->server_.isUser(arg.at(1))) {
		std::cerr << reply_.ERR_USERNOTINCHANNEL(arg.at(1), ch.getName())
				  << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_USERNOTINCHANNEL(arg.at(1), ch.getName()));
		return;
	}
	const User &target_user = this->server_.getUser(arg.at(1));
	if (!ch.isChannelUser(target_user.getFd())) {
		std::cerr << reply_.ERR_USERNOTINCHANNEL(target_user.getNickName(),
												 ch.getName())
				  << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), reply_.ERR_USERNOTINCHANNEL(target_user.getNickName(),
													  ch.getName()));
		return;
	}
	std::string broadcast_msg =
		"KICK " + target_user.getNickName() + " from " + arg.at(0);
	if (arg.size() >= 3 && arg.at(2)[0] == ':')
		broadcast_msg += extractAfterColon(3);
	this->server_.sendToChannelUser(ch.getName(), user, broadcast_msg);
	if (!ch.isChannelOperator(target_user.getFd()))
		const_cast<Channel &>(ch).removeChannelOperator(target_user.getFd());
	const_cast<User &>(target_user).removeChannel(ch.getName());
	const_cast<Channel &>(ch).removeUser(target_user.getFd());
	if (ch.getJoinedUserCount() == 0) {
		this->server_.removeChannel(ch.getName());
	}
}
