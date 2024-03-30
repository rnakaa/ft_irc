#ifndef USER_HPP
#define USER_HPP

#include "Channel.hpp"

#include <arpa/inet.h>
#include <cerrno>
#include <cstdlib> //exit, EXIT_FAILURE
#include <cstring> //strerror, strcpy, strlen
#include <iostream>
#include <map>
#include <poll.h>
#include <set>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Channel;

class User {
  public:
	enum AuthFlags { NONE_AUTH, PASS_AUTH, NICK_AUTH, USER_AUTH, ALL_AUTH };
	enum UserMode {
		none = 0,
		a = 1 << 1,
		i = 1 << 2,
		w = 1 << 3,
		r = 1 << 4,
		o = 1 << 5,
		O = 1 << 6,
		s = 1 << 7
	};

  public:
	User();
	User(int fd);

	int getFd() const;
	AuthFlags getAuthFlags() const;
	const std::string &getNickName() const;
	const std::string &getRealName() const;
	const std::string &getUserName() const;
	size_t getJoinedChannelCount() const;
	size_t getInvitedChannelCount() const;
	const std::set<std::string> &getJoinedChannels() const;
	const std::vector<std::string> &getInvitedChannels() const;
	enum UserMode getMode() const;
	void setAuthFlags(const AuthFlags &flags);
	void setNickname(const std::string &nickname);
	void setUsername(const std::string &username);
	void setRealName(const std::string &real_name);
	void setInvitedChannel(const std::string &ch_name);
	bool isUsernameSet() const;
	void setMode(const enum UserMode mode);
	void unsetMode(const enum UserMode mode);
	bool hasMode(const enum UserMode mode) const;
	void setChannel(const Channel &ch);

	bool isMemberOfChannel(const std::string &ch_name);

	void printJoinChannel() const;

	void removeChannel(const std::string &ch_name);
	void removeInvitedChannel(const std::string &ch_name);

  private:
	const int fd_;
	enum UserMode mode_;
	AuthFlags auth_flag_;
	std::string nick_name_;
	std::string user_name_;
	std::string real_name_;
	std::set<std::string> ch_set_;
	std::vector<std::string> invited_ch_;
	// std::map<std::string, Channel> ch_map_;
};

#endif
