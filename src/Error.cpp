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

std::string Error::ERR_CHANOPRIVSNEEDED(const std::string &ch_name) const {
	return ch_name + ":You're not channel operator";
}

std::string Error::ERR_UNKNOWNMODE(const std::string &c,
								   const std::string &ch_name) const {
	return c + ":is unknown mode char to me for " + ch_name;
}

std::string Error::ERR_NOPRIVILEGES() const {
	return ":Permission Denied- You're not an IRC operator";
}

std::string Error::ERR_UMODEUNKNOWNFLAG(const std::string &mode_flag) const {
	return mode_flag + ":Unknown MODE flag";
}
