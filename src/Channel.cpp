#include "Channel.hpp"

Channel::Channel() : ch_name_(""), ch_pass_("") {
	// std::cout << "Channel Constructor, ch_name_: " << this->ch_name_ <<
	// std::endl;
}

Channel::Channel(const std::string &ch_name) : ch_name_(ch_name), ch_pass_("") {
	// std::cout << "Channel Constructor, ch_name_: " << this->ch_name_ <<
	// std::endl;
}

Channel::Channel(const std::string &name, const std::string &pass)
	: ch_name_(name), ch_pass_(pass) {
	// std::cout << "Channel Constructor, ch_name_: " << this->ch_name_ <<
	// "ch_pass_: " << this->ch_pass_ << std::endl;
}

Channel::Channel(const std::string &name, const std::string &pass,
				 const User &user)
	: ch_name_(name), ch_pass_(pass) {
	setUser(user);
	// std::cout << "Channel Constructor with user, ch_name_: " <<
	// this->ch_name_
	// 		  << "ch_pass_: " << this->ch_pass_ << std::endl;
}

const std::string &Channel::getName() const { return (this->ch_name_); }

const std::string &Channel::getPass() const { return (this->ch_pass_); }

size_t Channel::getJoinedUserCount() const { return (this->ch_users_.size()); }

void Channel::setUser(const User &user) {
	// this->ch_users_.insert(std::make_pair(user.getFd(), &user));
	this->ch_users_[user.getFd()] = const_cast<User *>(&user);
}

enum Channel::ChannelMode Channel::getMode() const { return this->mode_; }

bool Channel::hasMode(const enum Channel::ChannelMode mode) const {
	return (mode & this->mode_) != 0;
}

void Channel::setMode(const enum Channel::ChannelMode mode) {
	this->mode_ = static_cast<enum ChannelMode>(this->mode_ | mode);
}

void Channel::printJoinedUser() const {
	std::cout << this->ch_name_ << " joined user: ";
	for (std::map<int, User *>::const_iterator it = ch_users_.begin();
		 it != ch_users_.end(); ++it) {
		std::cout << "client[" << it->first << "], ";
	}
	std::cout << std::endl;
}

void Channel::removeUser(const int fd) {
	printJoinedUser();
	std::map<int, User *>::iterator it = ch_users_.find(fd);
	if (it != ch_users_.end()) {
		ch_users_.erase(it);
	} else {
		std::cerr << "cannot remove client[" << fd << "]" << std::endl;
	}
	printJoinedUser();
}

std::map<int, User *>::const_iterator Channel::getMapIterator() const {
	return this->ch_users_.begin();
}
