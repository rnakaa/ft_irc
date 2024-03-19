#include "User.hpp"

User::User(const int fd) : fd_(fd), is_auth_(1), nick_name_("") {
	std::cout << "User fd_: " << this->fd_ << " ,is_auth_: " << this->is_auth_
			  << std::endl;
	(void)nick_name_;
}

int User::getFd() const { return (this->fd_); }

User::AuthFlags User::getAuthFlags() const { return (this->auth_flag_); }

void User::setAuthFrags(const AuthFlags &flags) { this->auth_flag_ = flags; }

User::User() : fd_(-1), is_auth_(1), nick_name_("") {}
