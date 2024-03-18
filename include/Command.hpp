#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include "User.hpp"
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>

class Server;
class User;

class Command {
  public:
	Command(Server &server);
	void executeCommand(const User &user);
	void parseClientMessage(const std::string &message);
	void handleCommand(const User &user, std::string &message);

  private:
	Server &server_;
	std::string command_name_;
	std::vector<std::string> arg_;
	typedef void (Command::*CommandFunction)(const User &,
											 std::vector<std::string> &);
	std::map<std::string, CommandFunction> commands_map_;

  private:
	void pass(const User &user, std::vector<std::string> &arg);
	void printTest(const User &user, std::vector<std::string> &arg);
};

#endif
