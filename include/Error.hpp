#ifndef ERROR_HPP
#define ERROR_HPP

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

class Error {
  public:
	std::string ERR_NEEDMOREPARAMS(const std::string &command) const;
	std::string ERR_ALREADYREGISTRED() const;
	std::string ERR_PASSWDMISMATCH() const;
	std::string ERR_NONICKNAMEGIVEN() const;
	std::string ERR_ERRONEUSNICKNAME(const std::string &nick) const;
	std::string ERR_NICKNAMEINUSE(const std::string &nick) const;
	std::string ERR_RESTRICTED() const;
	std::string ERR_NICKCOLLISION(const std::string &nick) const;
	std::string ERR_NOTSETPASS() const;
};

#endif
