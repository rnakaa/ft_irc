#include "Error.hpp"

std::string Error::ERR_ALREADYREGISTRED() const {
	return ":You may not reregister";
}

std::string Error::ERR_NEEDMOREPARAMS(const std::string &command) const {
	return command + " :Not enough parameters";
}

std::string Error::ERR_PASSWDMISMATCH() const { return ":Password incorrect"; }

std::string Error::ERR_NONICKNAMEGIVEN() const { return ":No nickname given"; }

std::string Error::ERR_ERRONEUSNICKNAME(const std::string &nick) const {
	return nick + " :Erroneous nickname";
}

std::string Error::ERR_NICKNAMEINUSE(const std::string &nick) const {
	return nick + " :Nickname is already in use";
}

std::string Error::ERR_RESTRICTED() const {
	return ":Your connection is restricted!";
}

std::string Error::ERR_NICKCOLLISION(const std::string &nick) const {
	return nick + " :Nickname collision";
}

std::string Error::ERR_NOTSETPASS() const { return ":Pass is not yet set."; }

std::string Error::ERR_NOSUCHCHANNEL(const std::string &ch_name) const {
	return ch_name + ":No such chaanel";
}

std::string Error::ERR_BADCHANNELKEY(const std::string &ch_name) const {
	return ch_name + ":Cannot join channel (+k)";
}
