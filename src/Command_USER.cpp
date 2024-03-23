#include "Command.hpp"

std::string concatRealName(std::vector<std::string> &arg) {
	std::string realname;
	realname = arg.at(3);
	size_t i = 0;

	if (realname[0] == ':') {
		if (realname != ":")
			realname = realname.substr(1, std::string::npos);
		else {
			if (arg.size() == 4)
				return "";
			realname = arg.at(4);
			i++;
		}
		for (i += 4; i < arg.size(); ++i) {
			realname += " " + arg.at(i);
		}
	}
	return realname;
}

void Command::USER(User &user, std::vector<std::string> &arg) {
	if (user.getAuthFlags() == User::NONE_AUTH) {
		std::cerr << error_.ERR_NOTSETPASS() << std::endl;
		server_.sendMsgToClient(user.getFd(), error_.ERR_NOTSETPASS());
	} else if (arg.size() < 4) {
		std::cerr << error_.ERR_NEEDMOREPARAMS("USER") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_NEEDMOREPARAMS("USER"));
	} else if (user.isUsernameSet()) {
		std::cerr << error_.ERR_ALREADYREGISTRED() << std::endl;
		server_.sendMsgToClient(user.getFd(), error_.ERR_ALREADYREGISTRED());
	} else {
		user.setUsername(arg.at(0));
		user.setRealName(concatRealName(arg));
		server_.sendMsgToClient(user.getFd(), "USER name success");
	}
}
