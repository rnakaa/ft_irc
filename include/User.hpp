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

  public:
	User();
	User(int fd);

	int getFd() const;
	AuthFlags getAuthFlags() const;

	void setAuthFrags(const AuthFlags &flags);

  private:
	const int fd_;
	AuthFlags auth_flag_;
	std::string nick_name_;
};

#endif
