#include "Channel.hpp"

Channel::Channel(const std::string &ch_name) : ch_name_(ch_name) {
	std::cout << "Channel Constructor, ch_name_: " << this->ch_name_
			  << std::endl;
}

Channel::Channel(const std::string &name, const std::string &pass)
	: ch_name_(name), ch_pass_(pass) {
	std::cout << "Channel Constructor, ch_name_: " << this->ch_name_
			  << "ch_pass_: " << this->ch_pass_ << std::endl;
}

Channel::Channel(const std::string &name, const std::string &pass,
				 const User &user)
	: ch_name_(name), ch_pass_(pass) {
	setUser(user);
	std::cout << "Channel Constructor with user, ch_name_: " << this->ch_name_
			  << "ch_pass_: " << this->ch_pass_ << std::endl;
}

const std::string &Channel::getName() const { return (this->ch_name_); }

const std::string &Channel::getPass() const { return (this->ch_pass_); }

void Channel::setUser(const User &user) {
	this->ch_users_.insert(std::make_pair(user.getFd(), user));
}
