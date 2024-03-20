#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Error.hpp"
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
	void PASS(User &user, std::vector<std::string> &arg);
	void TEST(User &user, std::vector<std::string> &arg);
	// void MOD(User &user, std::vector<std::string> &arg);
};

#endif
