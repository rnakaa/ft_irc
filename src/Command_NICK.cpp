#include "Command.hpp"

bool isSpecial(const char &c) {
	return c != '[' && c != ']' && c != '\\' && c != '^' && c != '_' &&
		   c != '`' && c != '{' && c != '|' && c != '}';
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

	if (user.getAuthFlags() == User::NONE_AUTH) {
		std::cerr << reply_.ERR_NOTSETPASS() << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_NOTSETPASS());
		return;
	} else if (arg.size() >= 2) {
		std::cerr << error_.ERR_TOOMANYPARAMS("NICK") << std::endl;
		server_.sendMsgToClient(user.getFd(), error_.ERR_TOOMANYPARAMS("NICK"));
		return;
	} else if (arg.empty()) {
		std::cerr << reply_.ERR_NONICKNAMEGIVEN() << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_NONICKNAMEGIVEN());
		return;
	} else if (user.hasMode(User::r)) { // Userにrステータスがあれば受け付けない
		std::cerr << reply_.ERR_RESTRICTED() << std::endl;
		server_.sendMsgToClient(user.getFd(), reply_.ERR_RESTRICTED());
		return;
	} else if (arg.at(0).length() > 9) { // NickNameが9文字以上ではないか確認
		std::cerr << reply_.ERR_ERRONEUSNICKNAME(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_ERRONEUSNICKNAME(arg.at(0)));
		return;
	}

	convertToScandinavian(
		arg.at(0)); // スカンディナビアンを満たすように文字列を変更

	if (!isalpha(arg.at(0)[0]) &&
		!isSpecial(arg.at(0)[0])) // 最初の文字のチェック
	{
		std::cerr << reply_.ERR_ERRONEUSNICKNAME(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_ERRONEUSNICKNAME(arg.at(0)));
	} else if (!checkStringValidity(arg.at(0))) { // 最初の文字以外のチェック
		std::cerr << reply_.ERR_ERRONEUSNICKNAME(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_ERRONEUSNICKNAME(arg.at(0)));
	} else if (server_.nicknameExist(arg.at(
				   0))) { // ニックネームが既に登録されているかどうかの確認
		std::cerr << reply_.ERR_NICKCOLLISION(arg.at(0)) << std::endl;
		server_.sendMsgToClient(user.getFd(),
								reply_.ERR_NICKCOLLISION(arg.at(0)));
	} else {
		server_.nicknameInsertLog(arg.at(0));
		user.setNickname(arg.at(0));
		if (user.getAuthFlags() == User::ALL_AUTH) {
			this->server_.sendMsgToClient(user.getFd(), "NICK rename success");
			return;
		} else if (user.getAuthFlags() == User::USER_AUTH) {
			user.setAuthFlags(User::ALL_AUTH);
			server_.sendMsgToClient(
				user.getFd(),
				reply_.RPL_WELCOME(user.getNickName(), user.getUserName()));
		} else {
			user.setAuthFlags(User::NICK_AUTH);
			server_.sendMsgToClient(user.getFd(), "NICK name success");
		}
		std::cout << user.getAuthFlags() << std::endl;
	}
}
