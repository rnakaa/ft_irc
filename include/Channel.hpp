#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "User.hpp"

#include <map>
#include <string>

class User;

class Channel {
	enum ChannelMode {
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
	void setUser(const User &user);
	enum ChannelMode getMode() const;
	void setMode(const enum ChannelMode mode);
	bool hasMode(const enum ChannelMode mode) const;

  private:
	std::string ch_name_;
	std::string ch_pass_;
	std::map<int, User> ch_users_;
	enum ChannelMode mode_;
};

#endif
