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
	enum AuthFlags { NONE_AUTH, PASS_AUTH, NICK_AUTH, USER_AUTH, ALL_AUTH };
	enum UserMode {
		a = 1 << 1,
		i = 1 << 2,
		w = 1 << 3,
		r = 1 << 4,
		o = 1 << 5,
		O = 1 << 6,
		s = 1 << 7
	};

  public:
	User();
	User(int fd);

	int getFd() const;
	AuthFlags getAuthFlags() const;
	void setAuthFlags(const AuthFlags &flags);
	enum UserMode getMode() const;
	void setNickname(const std::string &nickname);
	bool isUsernameSet() const;
	void setMode(const enum UserMode mode);
	bool hasMode(const enum UserMode mode) const;
	void setChannel(const std::string &ch_name, const Channel &ch);

	bool isMemberOfChannel(const std::string &ch_name);

	void printJoinChannel() const;

  private:
	const int fd_;
	enum UserMode mode_;
	AuthFlags auth_flag_;
	std::string nick_name_;
	std::string user_name_;
	std::map<std::string, Channel> ch_map_;
};

#endif
