#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Error.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <map>
#include <queue>
#include <sstream>
#include <unistd.h>
#include <vector>

class Server;
class User;
class Error;

class Command {
  public:
	Command(Server &server);
	void handleCommand(User &user, std::string &message);

  private:
	Server &server_;
	Error error_;
	std::string command_name_;
	std::vector<std::string> arg_;
	typedef void (Command::*CommandFunction)(User &,
											 std::vector<std::string> &);
	std::map<std::string, CommandFunction> commands_map_;

  private:
	void parseClientMessage(const std::string &message);

	// PASS
	void PASS(User &user, std::vector<std::string> &arg);

	// JOIN
	void JOIN(User &user, std::vector<std::string> &arg);
	bool startWithChannelChar(const std::string &str);
	bool setArgToVec(const std::vector<std::string> &arg,
					 std::queue<std::string> &ch_queue,
					 std::queue<std::string> &key_queue);
	bool checkValidArg(const std::queue<std::string> &ch_queue,
					   const std::queue<std::string> &key_queue);
	void handleChannelRequests(std::queue<std::string> &ch_queue,
							   std::queue<std::string> &key_queue, User &user);
	bool checkValidChannel(const std::string &ch_name);
	void joinChannel(const std::string &ch_name, const std::string &ch_key,
					 User &user);
	void createChannel(const std::string &ch_name, const std::string &ch_key,
					   User &user);

	void TEST(User &user, std::vector<std::string> &arg);
	// void MOD(User &user, std::vector<std::string> &arg);
};

#endif
