#include "Error.hpp"

std::string Error::ERR_ALREADYREGISTRED() { return ":You may not reregister"; }

std::string Error::ERR_NEEDMOREPARAMS(const std::string command) {
	return "<" + command + "> :Not enough parameters";
}

std::string Error::ERR_PASSWDMISMATCH() { return ":Password incorrect"; }