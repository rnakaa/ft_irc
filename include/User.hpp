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

enum mod { i = 1 << 1, s = 1 << 2, w = 1 << 3, o = 1 << 4 };

class User {
  public:
	enum AuthFlags { NONE_AUTH, PASS_AUTH, NICK_AUTH, ALL_AUTH };
	enum mod Mod;

  public:
	User();
	User(int fd);

	int getFd() const;
	AuthFlags getAuthFlags() const;
	void setAuthFrags(const AuthFlags &flags);
	enum mod getMod();
	void setMod(enum mod mod);
	bool checkMod(enum mod mod);

  private:
	const int fd_;
	AuthFlags auth_flag_;
	std::string nick_name_;
};

#endif
