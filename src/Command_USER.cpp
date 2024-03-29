#include "Command.hpp"
#include <cctype> //isspace

std::string Command::substrAfterColon(size_t i) const {
	size_t recv_size = recv_message_.size();
	if (recv_message_[recv_size - 1] == '\n')
		return recv_message_.substr(i + 1, recv_size - i - 2);
	return recv_message_.substr(i + 1, std::string::npos);
}

std::string Command::extractAfterColon(int words_to_skip) const {
	size_t i;
	int word_count = 0;

	for (i = 0; this->recv_message_[i]; i++) {
		while (!std::isspace(this->recv_message_[i]))
			i++;
		word_count++;
		while (std::isspace(this->recv_message_[i]))
			i++;
		if (word_count == words_to_skip)
			break;
	}
	return substrAfterColon(i);
}

std::string Command::extractRealName(std::vector<std::string> &arg) const {
	std::string realname = arg.at(3);

	if (realname[0] == ':')
		realname = extractAfterColon(4);
	return realname;
}

void Command::USER(User &user, std::vector<std::string> &arg) {
	if (user.getAuthFlags() == User::NONE_AUTH) {
		std::cerr << reply_.ERR_NOTSETPASS() << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_NOTSETPASS());
		return;
	} else if (user.getAuthFlags() == User::ALL_AUTH) {
		std::cerr << reply_.ERR_ALREADYREGISTRED() << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_ALREADYREGISTRED());
		return;
	} else if (arg.size() < 4) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("USER") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NEEDMOREPARAMS("USER"));
		return;
	} else if (user.isUsernameSet()) {
		std::cerr << reply_.ERR_ALREADYREGISTRED() << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_ALREADYREGISTRED());
		return;
	} else {
		user.setUsername(arg.at(0));
		user.setRealName(extractRealName(arg));
		if (user.getAuthFlags() == User::NICK_AUTH) {
			user.setAuthFlags(User::ALL_AUTH);
			server_.sendMsgToClient(
				user.getFd(),
				reply_.RPL_WELCOME(user.getNickName(), user.getUserName()));
		} else {
			user.setAuthFlags(User::USER_AUTH);
			server_.sendMsgToClient(user.getFd(), "USER name success");
		}
		std::cout << user.getAuthFlags() << std::endl;
	}
}
