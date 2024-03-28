#ifndef REPLY_HPP
#define REPLY_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <iostream>
#include <map>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Reply {
  public:
	// ERR
	std::string ERR_NEEDMOREPARAMS(const std::string &command) const;
	std::string ERR_ALREADYREGISTRED() const;
	std::string ERR_PASSWDMISMATCH() const;
	std::string ERR_NONICKNAMEGIVEN() const;
	std::string ERR_ERRONEUSNICKNAME(const std::string &nick) const;
	std::string ERR_NICKNAMEINUSE(const std::string &nick) const;
	std::string ERR_RESTRICTED() const;
	std::string ERR_NICKCOLLISION(const std::string &nick) const;
	std::string ERR_NOTSETPASS() const;
	std::string ERR_NOSUCHCHANNEL(const std::string &ch_name) const;
	std::string ERR_BADCHANNELKEY(const std::string &ch_name) const;
	std::string ERR_CHANOPRIVSNEEDED(const std::string &ch_name) const;
	std::string ERR_UNKNOWNMODE(const std::string &c,
								const std::string &ch_name) const;
	std::string ERR_NOPRIVILEGES() const;
	std::string ERR_UMODEUNKNOWNFLAG(const std::string &mode_flag) const;
	std::string ERR_USERNOTINCHANNEL(const std::string &nick_name,
									 const std::string &ch_name) const;
	std::string ERR_NOSUCHNICK(const std::string &nick) const;
	std::string ERR_TOOMANYPARAMS(const std::string &command) const;
	std::string ERR_CHANNELISFULL(const std::string &ch_name) const;
	std::string ERR_INVITEONLYCHAN(const std::string &ch_name) const;

	std::string ERR_NOTONCHANNEL(const std::string &ch_name) const;

	// RPL
	std::string RPL_WELCOME(const std::string &nick,
							const std::string &user) const;
	std::string RPL_NOTOPIC(const std::string &ch_name) const;
	std::string RPL_TOPIC(const std::string &ch_name,
						  const std::string &topic_str) const;
};

#endif
