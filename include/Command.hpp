#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Error.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <arpa/inet.h>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
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
	std::set<std::string> nickname_log_;

  private:
	void parseClientMessage(const std::string &message);

	// PASS
	void PASS(User &user, std::vector<std::string> &arg);

	// JOIN
	void JOIN(User &user, std::vector<std::string> &arg);
	void checkValidChannel(std::vector<std::string> &ch_vec,
						   std::vector<std::string> &key_vec);
	void setArgToVec(const std::vector<std::string> &arg,
					 std::vector<std::string> &ch_vec,
					 std::vector<std::string> &key_vec);
	bool checkValidChannel(const std::vector<std::string> &ch_vec,
						   const std::vector<std::string> &key_vec);
	// void joinChannel(const size_t i, std::vector<std::string> &ch_vec,
	// 				 std::vector<std::string> &key_vec);
	void joinChannel(const std::string &ch_name, User &user);
	void createChannel(const size_t i, std::vector<std::string> &ch_vec,
					   std::vector<std::string> &key_vec);
	// NICK
	void NICK(User &user, std::vector<std::string> &arg);
	bool containsNickname(const std::string &nickname) const;

	// USER
	void USER(User &user, std::vector<std::string> &arg);

	void TEST(User &user, std::vector<std::string> &arg);
	// void MOD(User &user, std::vector<std::string> &arg);
};

#endif
