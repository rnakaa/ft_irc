#include "User.hpp"

User::User(const int fd)
	: fd_(fd), mode_(none), auth_flag_(NONE_AUTH), nick_name_(""),
	  user_name_(""), real_name_("") {
	// std::cout << "User fd_: " << this->fd_ << " ,is_auth_: " << std::endl;
}

int User::getFd() const { return (this->fd_); }

User::AuthFlags User::getAuthFlags() const { return (this->auth_flag_); }

size_t User::getJoinedChannelCount() const { return (this->ch_set_.size()); }

const std::set<std::string> &User::getJoinedChannels() const {
	return (this->ch_set_);
}

void User::setAuthFlags(const AuthFlags &flags) { this->auth_flag_ = flags; }

void User::setChannel(const Channel &ch) {
	// this->ch_map_.insert(std::make_pair(ch_name, ch));
	this->ch_set_.insert(ch.getName());
}

bool User::isMemberOfChannel(const std::string &ch_name) {
	// return this->ch_map_.find(ch_name) != ch_map_.end();
	return this->ch_set_.find(ch_name) != ch_set_.end();
}

void User::printJoinChannel() const {
	std::cout << "client[" << getFd() << "] channel: ";
	for (std::set<std::string>::iterator it = ch_set_.begin();
		 it != ch_set_.end(); ++it) {
		std::cout << *it << ", ";
	}
	// for (std::map<std::string, Channel>::const_iterator it = ch_map_.begin();
	// 	 it != ch_map_.end(); ++it) {
	// 	std::cout << it->first << ", ";
	// }
	std::cout << std::endl;
}

User::User() : fd_(-1), auth_flag_(NONE_AUTH), nick_name_("") {}

enum User::UserMode User::getMode() const { return this->mode_; }

const std::string &User::getNickName() const { return this->nick_name_; }

const std::string &User::getRealName() const { return this->real_name_; }

const std::string &User::getUserName() const { return this->user_name_; }

bool User::hasMode(const enum User::UserMode mode) const {
	return (mode & this->mode_) != 0;
}

void User::setMode(const enum User::UserMode mode) {
	this->mode_ = (enum UserMode)(this->mode_ | mode);
}

void User::unsetMode(const enum User::UserMode mode) {
	this->mode_ = static_cast<enum UserMode>(this->mode_ - mode);
}

void User::setNickname(const std::string &nickname) {
	this->nick_name_ = nickname;
}

void User::setUsername(const std::string &username) {
	this->user_name_ = username;
}

void User::setRealName(const std::string &real_name) {
	this->real_name_ = real_name;
}

bool User::isUsernameSet() const { return !this->user_name_.empty(); }

void User::removeChannel(const std::string &ch_name) {
	this->ch_set_.erase(ch_name);
}
