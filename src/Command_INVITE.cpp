#include "Command.hpp"

void Command::INVITE(User &user, std::vector<std::string> &arg) {
	(void)arg;
	if (user.getAuthFlags() != User::ALL_AUTH) {
		std::cerr << "client cannot authenticate" << std::endl;
		server_.sendMsgToClient(user.getFd(), "client cannot authenticate");
		return;
	} else if (this->arg_.empty() || this->arg_.size() == 1) {
		std::cerr << error_.ERR_NEEDMOREPARAMS("INVITE") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_NEEDMOREPARAMS("INVITE"));
		return;
	} else if (this->arg_.size() > 2) {
		std::cerr << "INVITE: mode parameters are not required" << std::endl;
		this->server_.sendMsgToClient(
			user.getFd(), "INVITE: mode parameters are not required");
		return;
	} else if (!this->server_.hasChannelName(this->arg_.at(1))) {
		std::cerr << error_.ERR_NOSUCHCHANNEL(this->arg_.at(1)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_NOSUCHCHANNEL(this->arg_.at(1)));
		return;
	} else if (!this->server_.isUser(this->arg_.at(0))) {
		std::cerr << error_.ERR_NOSUCHNICK(this->arg_.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_NOSUCHNICK(this->arg_.at(0)));
		return;
	}
	const Channel &invite_ch = this->server_.getChannel(this->arg_.at(1));
	const User &invite_user = this->server_.getUser(this->arg_.at(0));
	if (!invite_ch.isChannelUser(user.getFd())) {
		std::cerr << invite_ch.getName() << " :You're not on that channel"
				  << std::endl;
		server_.sendMsgToClient(
			user.getFd(), invite_ch.getName() + " :You're not on that channel");
		return;
	} else if (invite_ch.hasMode(Channel::i) &&
			   !invite_ch.isChannelOperator(user.getFd())) {
		std::cerr << error_.ERR_CHANOPRIVSNEEDED(this->arg_.at(1)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_CHANOPRIVSNEEDED(this->arg_.at(1)));
		return;
	} else if (invite_ch.isChannelUser(invite_user.getFd())) {
		std::cerr << invite_user.getNickName() << " " << invite_ch.getName()
				  << " :is already on channel" << std::endl;
		server_.sendMsgToClient(user.getFd(), invite_ch.getName() +
												  " :is already on channel");
		return;
	}
	const_cast<Channel &>(invite_ch).setInvitedUser(invite_user.getFd());
	std::cout << invite_ch.getName() << " " << invite_user.getNickName()
			  << std::endl;
	this->server_.sendMsgToClient(user.getFd(), invite_ch.getName() + " " +
													invite_user.getNickName());
	this->server_.sendMsgToClient(invite_user.getFd(),
								  invite_ch.getName() + " " +
									  invite_user.getNickName());
}
