#include "Error.hpp"

std::string Error::ERR_ALREADYREGISTRED() {
	return ":You may not reregister\n";
}

std::string Error::ERR_NEEDMOREPARAMS(const std::string command) {
	return "<" + command + "> :Not enough parameters\n";
}

std::string Error::ERR_PASSWDMISMATCH() { return ":Password incorrect\n"; }
