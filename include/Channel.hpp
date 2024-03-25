#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "User.hpp"

#include <algorithm>
#include <map>

class User;

class Channel {
  public:
	enum ChannelMode {
		none = 0,
		O = 1 << 1,
		o = 1 << 2,
		v = 1 << 3,
		a = 1 << 4,
		i = 1 << 5,
		m = 1 << 6,
		n = 1 << 7,
		q = 1 << 8,
		p = 1 << 9,
		s = 1 << 10,
		t = 1 << 11,
		r = 1 << 12,
		k = 1 << 13,
		l = 1 << 14,
		b = 1 << 15,
		e = 1 << 16,
		I = 1 << 17
	};

  public:
	Channel();
	Channel(const std::string &name);
	Channel(const std::string &name, const std::string &pass);
	Channel(const std::string &name, const std::string &pass, const User &user);
	const std::string &getName() const;
	const std::string &getPass() const;
	const std::vector<std::string> &getChannelOperators() const;
	size_t getJoinedUserCount() const;
	const std::string &getCreatedUser() const;
	void setUser(const User &user);
	void setChannelOperator(const std::string &user_nickname);
	void removeChannelOperator(const std::string &user_nickname);
	enum ChannelMode getMode() const;
	void setMode(const enum ChannelMode mode);
	bool hasMode(const enum ChannelMode mode) const;
	void removeUser(const int fd);
	void printJoinedUser() const;
	void printChannelOperators() const;
	bool isChannelOperator(const std::string &nick_name) const;
	bool isChannelUser(const std::string &nick_name) const;

  private:
	std::string ch_name_;
	std::string ch_pass_;
	std::map<int, User> ch_users_;
	enum ChannelMode mode_;
	std::string created_user_;
	std::vector<std::string> ch_operators_;
};

#endif
