#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "Command.hpp"
#include "Server.hpp"
#include "User.hpp"

#include <map>
#include <string>

class User;

class Channel {
  public:
	Channel(const std::string &name);
	Channel(const std::string &name, const std::string &pass);
	Channel(const std::string &name, const std::string &pass, const User &user);
	const std::string &getName() const;
	const std::string &getPass() const;
	void setUser(const User &user);

  private:
	std::string ch_name_;
	std::string ch_pass_;
	std::map<int, User> ch_users_;
};

#endif
