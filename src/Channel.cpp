#include "Channel.hpp"

Channel::Channel() : ch_name_(""), ch_pass_(""), created_user_fd_(-1) {
	// std::cout << "Channel Constructor, ch_name_: " << this->ch_name_ <<
	// std::endl;
}

Channel::Channel(const std::string &ch_name)
	: ch_name_(ch_name), ch_pass_(""), created_user_fd_(-1) {
	// std::cout << "Channel Constructor, ch_name_: " << this->ch_name_ <<
	// std::endl;
}

Channel::Channel(const std::string &name, const std::string &pass)
	: ch_name_(name), ch_pass_(pass), created_user_fd_(-1) {
	// std::cout << "Channel Constructor, ch_name_: " << this->ch_name_ <<
	// "ch_pass_: " << this->ch_pass_ << std::endl;
}

Channel::Channel(const std::string &name, const std::string &pass,
				 const User &user)
	: ch_name_(name), ch_pass_(pass), created_user_fd_(user.getFd()) {
	setUser(user);
	// std::cout << "Channel Constructor with user, ch_name_: " <<
	// this->ch_name_
	// 		  << "ch_pass_: " << this->ch_pass_ << std::endl;
}

const std::string &Channel::getName() const { return (this->ch_name_); }

const std::string &Channel::getPass() const { return (this->ch_pass_); }

const std::vector<int> &Channel::getChannelOperators() const {
	return this->ch_operators_;
}

size_t Channel::getJoinedUserCount() const { return (this->ch_users_.size()); }

const std::string &Channel::getCreatedUser() const {
	const User *user = this->ch_users_.at(this->created_user_fd_);
	return user->getNickName();
}

const std::vector<std::string> Channel::getChannelOperatorsNickName() const {
	std::vector<std::string> ch_operators_nick;
	for (size_t i = 0; i < this->ch_operators_.size(); ++i) {
		ch_operators_nick.push_back(
			this->ch_users_.at(this->ch_operators_.at(i))->getNickName());
	}
	return ch_operators_nick;
}

void Channel::setUser(const User &user) {
	// this->ch_users_.insert(std::make_pair(user.getFd(), &user));
	this->ch_users_[user.getFd()] = const_cast<User *>(&user);
}

void Channel::setChannelOperator(const int user_fd) {
	this->ch_operators_.push_back(user_fd);
}

void Channel::setPass(const std::string &pass) { this->ch_pass_ = pass; }

void Channel::removeChannelOperator(const int user_fd) {
	for (std::vector<int>::iterator it = this->ch_operators_.begin();
		 it != this->ch_operators_.end(); ++it) {
		if (*it == user_fd) {
			it = this->ch_operators_.erase(it);
			return;
		}
	}
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

void Channel::printChannelOperators() const {
	std::cout << this->ch_name_ << " channel operators: ";
	for (size_t i = 0; i < this->ch_operators_.size(); ++i) {
		std::cout << "client[" << ch_operators_[i] << "], ";
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

bool Channel::isChannelOperator(const int user_fd) const {
	return std::find(this->ch_operators_.begin(), this->ch_operators_.end(),
					 user_fd) != ch_operators_.end();
}

bool Channel::isChannelUser(const int user_fd) const {
	for (std::map<int, User *>::const_iterator it = ch_users_.begin();
		 it != ch_users_.end(); ++it) {
		if (user_fd == it->second->getFd()) {
			return true;
		}
	}
	return false;
}
