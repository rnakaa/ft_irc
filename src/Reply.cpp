#include "Reply.hpp"

std::string Reply::ERR_ALREADYREGISTRED() const {
	return ":localhost 462 :You may not reregister";
}

std::string Reply::ERR_NEEDMOREPARAMS(const std::string &command) const {
	return ":localhost 461 " + command + " :Not enough parameters";
}

std::string Reply::ERR_PASSWDMISMATCH() const {
	return ":localhost 464 :Password incorrect";
}

std::string Reply::ERR_NONICKNAMEGIVEN() const {
	return ":localhost 431 :No nickname given";
}

std::string Reply::ERR_ERRONEUSNICKNAME(const std::string &nick) const {
	return ":localhost 432 " + nick + " :Erroneous nickname";
}

std::string Reply::ERR_NICKNAMEINUSE(const std::string &nick) const {
	return ":localhost 433 " + nick + " :Nickname is already in use";
}

std::string Reply::ERR_RESTRICTED() const {
	return ":localhost 484 :Your connection is restricted!";
}

std::string Reply::ERR_NICKCOLLISION(const std::string &nick) const {
	return ":localhost 436 " + nick +
		   " :Nickname collision KILL from localhost";
}

std::string Reply::ERR_NOTSETPASS() const { return ":Pass is not yet set"; }

std::string Reply::ERR_NOSUCHCHANNEL(const std::string &ch_name) const {
	return ":localhost 403 " + ch_name + " :No such channel";
}

std::string Reply::ERR_BADCHANNELKEY(const std::string &nick_name,
									 const std::string &ch_name) const {
	return ":localhost 475 " + nick_name + " " + ch_name +
		   " :Cannot join channel (+k)";
}

std::string Reply::ERR_CHANOPRIVSNEEDED(const std::string &ch_name) const {
	return ":localhost 482 " + ch_name + " :You're not channel operator";
}

std::string Reply::ERR_UNKNOWNMODE(const std::string &c,
								   const std::string &ch_name) const {
	return ":localhost 472 " + c + " :is unknown mode char to me for " +
		   ch_name;
}

std::string Reply::ERR_NOPRIVILEGES() const {
	return ":localhost 481 :Permission Denied- You're not an IRC operator";
}

std::string Reply::ERR_UMODEUNKNOWNFLAG(const std::string &mode_flag) const {
	return ":localhost 501 " + mode_flag + " :Unknown MODE flag";
}

std::string Reply::ERR_USERNOTINCHANNEL(const std::string &nick_name,
										const std::string &ch_name) const {
	return ":localhost 441 " + nick_name + " " + ch_name +
		   " They aren't on that channel";
}

std::string Reply::ERR_NOSUCHNICK(const std::string &nick) const {
	return ":localhost 401 " + nick + " :No such nick";
}

std::string Reply::RPL_WELCOME(const std::string &nickname,
							   const std::string &username) const {
	std::string result;
	char dateStr[100];
	std::time_t t = std::time(NULL);
	std::strftime(dateStr, sizeof(dateStr), "%b %d", std::localtime(&t));

	std::ostringstream ss;
	ss << "001 " << nickname << " :Welcome to the Internet Relay Network "
	   << nickname << "!" << username << "@localhost\r\n"
	   << "002 " << nickname
	   << " :Your host is localhost, running version 2.0\r\n"
	   << "003 " << nickname << " :This server was created " << dateStr
	   << "\r\n"
	   << "004 " << nickname << " :localhost 2.0 aiwroOs aimnqpsrtklbeI\r\n";
	result = ss.str();
	return result;
}
std::string Reply::ERR_CHANNELISFULL(const std::string &ch_name) const {
	return ":localhost 471 " + ch_name + " :Cannot join channel (+l)";
}

std::string Reply::ERR_TOOMANYPARAMS(const std::string &command) const {
	return command + " :too many params";
}

std::string Reply::ERR_INVITEONLYCHAN(const std::string &ch_name) const {
	return ":localhost 473 " + ch_name + " :Cannot join channel (+i)";
}

std::string Reply::ERR_NOTONCHANNEL(const std::string &ch_name) const {
	return ":localhost 442 " + ch_name + " :You're not on that channel";
}

// RPL

std::string Reply::RPL_NOTOPIC(const std::string &ch_name) const {
	return ":localhost 331 " + ch_name + " :No topic is set";
}

std::string Reply::RPL_TOPIC(const std::string &ch_name,
							 const std::string &topic_str) const {
	return ":localhost 332 " + ch_name + " :" + topic_str;
}

std::string Reply::RPL_YOUREOPER() const {
	return ":localhost 381 :You are now an IRC operator";
}

std::string Reply::RPL_KILL(const std::string &nick,
							const std::string &killed_user,
							const std::string &kill_str) const {
	return nick + " use KILL command to " + killed_user + ": " + kill_str;
}
