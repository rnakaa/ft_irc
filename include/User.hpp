#ifndef USER_HPP
#define USER_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> //exit, EXIT_FAILURE
#include <cstring> //strerror, strcpy, strlen
#include <iostream>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class User {
  public:
	enum AuthFlags { NONE_AUTH, PASS_AUTH, NICK_AUTH, ALL_AUTH };
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
	void setAuthFrags(const AuthFlags &flags);
	enum UserMode getMode() const;
	void setMode(const enum UserMode mode);
	bool hasMode(const enum UserMode mode) const;

  private:
	const int fd_;
	enum UserMode mode_;
	AuthFlags auth_flag_;
	std::string nick_name_;
};

#endif
