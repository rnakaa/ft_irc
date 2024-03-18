#ifndef USER_HPP
#define USER_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <iostream>
#include <poll.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class User {
  public:
	User(int fd);

  private:
	const int fd_;
	int is_auth_;
	std::string nick_name_;
};

#endif
