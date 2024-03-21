#include "Command.hpp"

bool isSpecial(const char &chr) {
	std::set<char> specialChars;
	specialChars.insert('[');
	specialChars.insert(']');
	specialChars.insert('\\');
	specialChars.insert('^');
	specialChars.insert('_');
	specialChars.insert('`');
	specialChars.insert('{');
	specialChars.insert('|');
	specialChars.insert('}');
	if (specialChars.find(chr) != specialChars.end())
		return true;
	return false;
}

bool checkStringValidity(const std::string &str) {
	for (std::string::size_type i = 1; i < str.length(); ++i) {
		if (!isalpha(str[i]) && !isdigit(str[i]) && !isSpecial(str[i]) &&
			!(str[i] == '-')) {
			return false;
		}
	}
	return true;
}

void convertToScandinavian(std::string &str) {
	for (std::string::size_type i = 0; i < str.length(); ++i) {
		if (str[i] == '{') {
			str[i] = '[';
		} else if (str[i] == '}') {
			str[i] = ']';
		} else if (str[i] == '\\') {
			str[i] = '|';
		}
	}
}

void Command::NICK(User &user, std::vector<std::string> &arg) {
	std::cout << "start nick " << user.getFd() << std::endl;

	if (arg.empty()) {
		std::cerr << error_.ERR_NONICKNAMEGIVEN() << std::endl;
		server_.sendMsgToClient(user.getFd(), error_.ERR_NONICKNAMEGIVEN());
	} else if (user.hasMode(User::r)) { // Userにrステータスがあれば受け付けない
		std::cerr << error_.ERR_RESTRICTED() << std::endl;
		server_.sendMsgToClient(user.getFd(), error_.ERR_RESTRICTED());
	} else if (arg.at(0).length() > 9) {
		std::cerr << error_.ERR_ERRONEUSNICKNAME(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_ERRONEUSNICKNAME(arg.at(0)));
	}

	convertToScandinavian(
		arg.at(0)); // スカンディナビアンを満たすように文字列を変更

	if (!isalpha(arg.at(0)[0]) &&
		!isSpecial(arg.at(0)[0])) // 最初の文字のチェック
	{
		std::cerr << error_.ERR_ERRONEUSNICKNAME(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_ERRONEUSNICKNAME(arg.at(0)));
	} else if (!checkStringValidity(arg.at(0))) { // 最初の文字以外のチェック
		std::cerr << error_.ERR_ERRONEUSNICKNAME(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_ERRONEUSNICKNAME(arg.at(0)));
	} else if (nickAlreadyExist(arg.at(0))) {
		std::cerr << error_.ERR_NICKCOLLISION(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_NICKCOLLISION(arg.at(0)));
	} else {
		this->nickname_log_.insert(arg.at(0));
		user.setNickname(arg.at(0));
		user.setAuthFrags(User::NICK_AUTH);
	}
}
