#include "Command.hpp"

void Command::PING(User &user, std::vector<std::string> &arg) {
	(void)user;
	(void)arg;
	server_.sendMsgToClient(user.getFd(), ":ft_ircserv PONG ft_ircserv " +
											  user.getNickName());
}
