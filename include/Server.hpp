#ifndef SERVER_HPP
#define SERVER_HPP

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

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024
#define POLL_WAIT_FOREVER -1
#define MAX_MSG_LEN 510

class Server {
  public:
	Server(const int argc, const char **argv);
	~Server();
	void init();
	void run();

  private:
	void checkValidArgc(const int argc) const;
	void checkValidPort(const char *str) const;
	void setPortAndPass(const char **argv);
	void exit_error(const std::string &func, const std::string &err_msg);
	std::string recvCmdFromClient(const size_t i);
	void acceptNewClientConnect();
	void handlPollEvents();

  private:
	std::string port_;
	std::string pass_;
	int server_sockfd_;
	struct sockaddr_in server_addr_;
	struct sockaddr_in client_addr_;
	std::string recv_msg_;
	std::vector<struct pollfd> pollfd_vec_;
	std::map<int, User> user_map_;
};

#endif
