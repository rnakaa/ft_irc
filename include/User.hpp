#ifndef USER_HPP
#define USER_HPP

#include "Channel.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> //exit, EXIT_FAILURE
#include <cstring> //strerror, strcpy, strlen
#include <iostream>
#include <map>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Channel;

class User {
  public:
	enum AuthFlags { NONE_AUTH, PASS_AUTH, NICK_AUTH, ALL_AUTH };

  public:
	User();
	User(int fd);

	int getFd() const;
	AuthFlags getAuthFlags() const;

	void setAuthFrags(const AuthFlags &flags);
	void setChannel(const std::string &ch_name, const Channel &ch);

  private:
	const int fd_;
	AuthFlags auth_flag_;
	std::string nick_name_;
	std::map<std::string, Channel> ch_map_;
};

#endif
