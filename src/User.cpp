#include "User.hpp"

User::User(const int fd) : fd_(fd), auth_flag_(NONE_AUTH), nick_name_("") {
	std::cout << "User fd_: " << this->fd_ << " ,is_auth_: " << std::endl;
	(void)nick_name_;
}

int User::getFd() const { return (this->fd_); }

User::AuthFlags User::getAuthFlags() const { return (this->auth_flag_); }

void User::setAuthFrags(const AuthFlags &flags) { this->auth_flag_ = flags; }

User::User() : fd_(-1), auth_flag_(NONE_AUTH), nick_name_("") {}

enum User::UserMod User::getMod() const { return this->mod_; }

bool User::hasMod(const enum User::UserMod mod) const {
	return (mod & this->mod_) != 0;
}

void User::setMod(const enum User::UserMod mod) {
	this->mod_ = (enum UserMod)(this->mod_ | mod);
}
