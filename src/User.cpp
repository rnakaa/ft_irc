#include "User.hpp"

User::User(const int fd) : fd_(fd), auth_flag_(NONE_AUTH), nick_name_("") {
	// std::cout << "User fd_: " << this->fd_ << " ,is_auth_: " << std::endl;
	(void)nick_name_;
}

int User::getFd() const { return (this->fd_); }

User::AuthFlags User::getAuthFlags() const { return (this->auth_flag_); }

void User::setAuthFrags(const AuthFlags &flags) { this->auth_flag_ = flags; }

void User::setChannel(const std::string &ch_name, const Channel &ch) {
	this->ch_map_.insert(std::make_pair(ch_name, ch));
}

bool User::isMemberOfChannel(const std::string &ch_name) {
	return this->ch_map_.find(ch_name) != ch_map_.end();
}

void User::printJoinChannel() const {
	std::cout << "client[" << getFd() << "] channel: ";
	for (std::map<std::string, Channel>::const_iterator it = ch_map_.begin();
		 it != ch_map_.end(); ++it) {
		std::cout << it->first << ", ";
	}
	std::cout << std::endl;
}

User::User() : fd_(-1), auth_flag_(NONE_AUTH), nick_name_("") {}

enum User::UserMode User::getMode() const { return this->mode_; }

bool User::hasMode(const enum User::UserMode mode) const {
	return (mode & this->mode_) != 0;
}

void User::setMode(const enum User::UserMode mode) {
	this->mode_ = (enum UserMode)(this->mode_ | mode);
}
