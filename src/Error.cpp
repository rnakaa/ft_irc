#include "Error.hpp"

std::string Error::ERR_ALREADYREGISTRED() { return ":You may not reregister"; }

std::string Error::ERR_NEEDMOREPARAMS(const std::string command) {
	return "<" + command + "> :Not enough parameters";
}

std::string Error::ERR_PASSWDMISMATCH() { return ":Password incorrect"; }

std::string Error::ERR_NOSUCHCHANNEL(const std::string &ch_name) {
	return ch_name + ":No such chaanel";
}

std::string Error::ERR_BADCHANNELKEY(const std::string &ch_name) {
	return ch_name + ":Cannot join channel (+k)";
}
