#ifndef ERROR_HPP
#define ERROR_HPP

#include "User.hpp"
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
};

#endif