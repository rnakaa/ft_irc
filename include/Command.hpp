#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "Reply.hpp"
#include "Server.hpp"
#include "User.hpp"
#include <arpa/inet.h>
#include <cctype>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <set>
#include <sstream>
#include <unistd.h>
#include <vector>

class Server;
class User;
class Reply;

class Command {
  public:
	Command(Server &server);
	void handleCommand(User &user, std::string &message);

  private:
	enum ModeAction { setMode, unsetMode, queryMode };

	Server &server_;
	Reply reply_;
	std::string command_name_;
	std::vector<std::string> arg_;
	std::string recv_message_;
	typedef void (Command::*CommandFunction)(User &,
											 std::vector<std::string> &);
	std::map<std::string, CommandFunction> commands_map_;
	typedef void (Command::*ModeFunction)(const ModeAction, User &,
										  const Channel &);
	std::map<char, ModeFunction> mode_map_;

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
	void exitAllChannels(User &user);
	// NICK
	void NICK(User &user, std::vector<std::string> &arg);
	bool containsNickname(const std::string &nickname) const;
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

	// USER
	void USER(User &user, std::vector<std::string> &arg);
	std::string extractRealName(std::vector<std::string> &arg) const;
	std::string extractAfterColon(int words_to_skip) const;
	std::string substrAfterColon(size_t i) const;

	// QUIT
	void QUIT(User &user, std::vector<std::string> &arg);
	void quitAllChannels(User &user, std::string broadcast_msg);

	// TEST
	void TEST(User &user, std::vector<std::string> &arg);

	// INVITE
	void INVITE(User &user, std::vector<std::string> &arg);

	// TOPIC
	void TOPIC(User &user, std::vector<std::string> &arg);
	void queryChannelTopic(User &user, const Channel &topic_ch);
	void removeChannelTopic(User &user, const Channel &topic_ch);
	void setChannelTopic(User &user, const Channel &topic_ch);

	// MODE
	void MODE(User &user, std::vector<std::string> &arg);
	void handleChannelMode(User &user, std::vector<std::string> &arg,
						   const Channel &ch_name);
	ModeAction checkModeAction(const std::string &mode_str) const;
	bool checkModeType(const char c) const;
	bool checkInvalidSignsCount(const std::string &mode_str);
	void joinStrFromVector(std::string &join_str, const Channel &ch,
						   const std::string delimiter);
	// mode 'O'
	void handleChannelOriginOperator(const ModeAction mode_action, User &user,
									 const Channel &ch);
	// mode 'o'
	void handleChannelOperator(const ModeAction mode_action, User &user,
							   const Channel &ch);
	void setOrUnsetChannelOperator(const size_t i, const ModeAction mode_action,
								   User &user, const Channel &ch);
	// mode 'k'
	void handleChannelKey(const ModeAction mode_action, User &user,
						  const Channel &ch);
	void handleKeyQueryMode(User &user, const Channel &ch);
	void handleKeySetMode(User &user, const Channel &ch);
	void handleKeyUnsetMode(User &user, const Channel &ch);
	// mode 'l'
	void handleLimitedUserNum(const ModeAction mode_action, User &user,
							  const Channel &ch);
	void handleLimitedQueryMode(User &user, const Channel &ch);
	void handleLimitedSetMode(User &user, const Channel &ch);
	void handleLimitedUnsetMode(User &user, const Channel &ch);
	// PRIVMSG
	void PRIVMSG(User &user, std::vector<std::string> &arg);
	void sendMessage(User &user, const std::string &dsn,
					 const std::string &msg);
	// mode 'i'
	void handleInviteOnly(const ModeAction mode_action, User &user,
						  const Channel &ch);
	void queryInviteOnly(User &user, const Channel &ch);
	void setInviteOnly(User &user, const Channel &ch);
	void unsetInviteOnly(User &user, const Channel &ch);
	// mode 't'
	void handleTopicOnlyOperator(const ModeAction mode_action, User &user,
								 const Channel &ch);

	// PING
	void PING(User &user, std::vector<std::string> &arg);
};

#endif
