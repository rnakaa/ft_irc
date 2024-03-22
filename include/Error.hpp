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
	std::string ERR_NEEDMOREPARAMS(const std::string command);
	std::string ERR_ALREADYREGISTRED();
	std::string ERR_PASSWDMISMATCH();
	std::string ERR_NOSUCHCHANNEL(const std::string &ch_name);
	std::string ERR_BADCHANNELKEY(const std::string &ch_name);
};

#endif
