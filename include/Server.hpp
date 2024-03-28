#ifndef SERVER_HPP
#define SERVER_HPP

#include "Channel.hpp"
#include "Command.hpp"
#include "User.hpp"
#include <arpa/inet.h>
#include <cerrno>
#include <iostream>
#include <map>
#include <poll.h>
#include <set>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#define PORT 6667
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024
#define POLL_WAIT_FOREVER -1
#define MAX_MSG_LEN 510

class User;
class Channel;

class Server {
  public:
	Server(const int argc, const char **argv);
	~Server();
	void init();
	void run();

	const std::string &getPass() const;
	const Channel &getChannel(const std::string &ch_name) const;
	const User &getUser(const std::string &nickname) const;
	void printChannelName() const;

	void setChannel(const std::string &ch_name, const Channel &ch);

	bool hasChannelName(const std::string &ch_name) const;
	bool isUser(const std::string &nickname) const;

	void sendMsgToClient(const int fd, const std::string &send_str) const;

	bool nicknameExist(const std::string &nickname) const;
	void nicknameInsertLog(std::string nickname);

	void removeChannel(const std::string &ch_name);

	void sendToChannelUser(std::string &ch_name, const std::string &msg) const;

  private:
	void checkValidArgc(const int argc) const;
	void checkValidPort(const char *str) const;
	void setPortAndPass(const char **argv);
	void exit_error(const std::string &func, const std::string &err_msg) const;
	std::string recvCmdFromClient(const size_t i);
	void acceptNewClientConnect();
	void handlPollEvents();

  private:
	int port_;
	std::string pass_;
	int server_sockfd_;
	struct sockaddr_in server_addr_;
	struct sockaddr_in client_addr_;
	std::string recv_msg_;
	std::vector<struct pollfd> pollfd_vec_;
	std::map<int, User> user_map_;
	std::map<std::string, Channel> ch_map_;
	std::set<std::string> nickname_log_;
};

#endif
