#include "Error.hpp"

std::string Error::ERR_ALREADYREGISTRED() { return ":You may not reregister"; }

std::string Error::ERR_NEEDMOREPARAMS(const std::string command) {
	return "<" + command + "> :Not enough parameters";
}

std::string Error::ERR_PASSWDMISMATCH() { return ":Password incorrect"; }

std::string Error::ERR_NONICKNAMEGIVEN() { return ":No nickname given"; }

std::string Error::ERR_ERRONEUSNICKNAME(std::string nick) {
	return "<" + nick + "> :Erroneous nickname";
}

std::string Error::ERR_NICKNAMEINUSE(std::string nick) {
	return "<" + nick + "> :Nickname is already in use";
}

std::string Error::ERR_RESTRICTED() {
	return ":Your connection is restricted!";
}

std::string Error::ERR_NICKCOLLISION(std::string nick) {
	return "<" + nick + "> :Nickname collision";
}
