#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
#include <cerrno>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024

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

  private:
	std::string port_;
	std::string pass_;
	int server_sockfd_;
	struct sockaddr_in server_addr_;
	// struct sockaddr_in client_addr_;
	// char recv_msg_[BUF_SIZE];
};

#endif
