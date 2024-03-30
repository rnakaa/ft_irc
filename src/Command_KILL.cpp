#include "Command.hpp"

void Command::KILL(User &user, std::vector<std::string> &arg) {
	if (arg.size() < 2) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("KILL") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NEEDMOREPARAMS("KILL"));
		return;
	} else if (!user.hasMode(User::o)) {
		std::cerr << reply_.ERR_NOPRIVILEGES() << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_NOPRIVILEGES());
		return;
	} else if (!this->server_.isUser(arg.at(0))) {
		std::cerr << reply_.ERR_NOSUCHNICK(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_NOSUCHNICK(arg.at(0)));
		return;
	}
	std::string kill_comment;
	for (size_t i = 1; i < arg.size(); ++i) {
		kill_comment += arg.at(i);
		if (i < arg.size() - 1) {
			kill_comment += " ";
		}
	}
	// std::cout << "kill_comment: " << kill_comment << std::endl;
	const User &kill_user = this->server_.getUser(arg.at(0));
	const int fd = kill_user.getFd();
	std::cout << reply_.RPL_KILL(user.getNickName(), kill_user.getNickName(),
								 kill_comment)
			  << std::endl;
	this->server_.sendMsgToClient(fd, reply_.RPL_KILL(user.getNickName(),
													  kill_user.getNickName(),
													  kill_comment));
	removeAllInvitedChannels(const_cast<User &>(kill_user));
	quitAllChannels(const_cast<User &>(kill_user), kill_comment);
	this->server_.removeUser(fd);
	this->server_.removePollfd(fd);
	close(fd);
}
