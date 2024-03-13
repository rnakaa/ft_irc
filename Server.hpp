#ifndef SERVER_HPP
#define SERVER_HPP

#include <arpa/inet.h>
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

  private:
	std::string port_;
	std::string pass_;
	// int server_sockfd_;
	// struct sockaddr_in server_addr;
	// struct sockaddr_in client_addr;
	// char recv_msg[BUF_SIZE];
};

#endif
