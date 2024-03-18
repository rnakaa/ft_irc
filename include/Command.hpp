#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Server.hpp"
#include <arpa/inet.h>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <unistd.h>
#include <vector>

typedef void (*CommandFunction)(const int, std::vector<std::string> &);

class Command {
  public:
	Command(Server &server);
	void executeCommand(const int fd);
	void parseClientMessage(std::string &message);
	void handleCommand(const int fd, std::string &message);

  private:
	Server &server_;
	std::vector<std::string> arg_;
	std::map<std::string, CommandFunction> commands_map_;
	std::string command_name_;

  private:
	static void pass(const int fd, std::vector<std::string> &arg);
};

#endif
