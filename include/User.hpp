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
	enum UserMod { i = 1 << 1, s = 1 << 2, w = 1 << 3, o = 1 << 4 };

  public:
	User();
	User(int fd);

	int getFd() const;
	AuthFlags getAuthFlags() const;
	void setAuthFrags(const AuthFlags &flags);
	enum UserMod getMod() const;
	void setMod(const enum UserMod mod);
	bool hasMod(const enum UserMod mod) const;

  private:
	const int fd_;
	enum UserMod mod_;
	AuthFlags auth_flag_;
	std::string nick_name_;
};

#endif
