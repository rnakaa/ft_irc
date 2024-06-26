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
	const std::vector<int> &getChannelOperators() const;
	size_t getJoinedUserCount() const;
	size_t getInvitedUsersCount() const;
	const std::vector<std::string> getChannelOperatorsNickName() const;
	const std::vector<int> &getInvitedUsers() const;
	const std::string getCreatedUser() const;
	const ssize_t &getMaxUsers() const;
	const std::string &getTopicStr() const;
	void setUser(const User &user);
	void setPass(const std::string &pass);
	void setChannelOperator(const int user_fd);
	void setMaxUsers(const int max_users);
	void setInvitedUser(const int user_fd);
	void setTopicStr(const std::string &str);
	void setChannelCreater(const int user_fd);
	void removeChannelOperator(const int user_fd);
	enum ChannelMode getMode() const;
	void setMode(const enum ChannelMode mode);
	void unsetMode(const enum ChannelMode mode);
	bool hasMode(const enum ChannelMode mode) const;
	void removeUser(const int fd);
	void printJoinedUser() const;
	std::map<int, User *>::const_iterator getMapBeginIterator() const;
	std::map<int, User *>::const_iterator getMapEndIterator() const;
	void printChannelOperators() const;
	bool isChannelOperator(const int user_fd) const;
	bool isChannelUser(const int user_fd) const;
	bool isInvitedUser(const int user_fd) const;
	bool isChannelCreater(const int user_fd) const;
	void removeInvitedUser(const int user_fd);

  private:
	std::string ch_name_;
	std::string ch_pass_;
	std::map<int, User *> ch_users_;
	enum ChannelMode mode_;
	int created_user_fd_;
	std::vector<int> ch_operators_;
	std::vector<int> invited_users_;
	ssize_t max_users_;
	std::string topic_str_;
};

#endif
