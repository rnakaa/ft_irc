#include "Command.hpp"

void Command::OPER(User &user, std::vector<std::string> &arg) {
	if (arg.size() < 2) {
		std::cerr << reply_.ERR_NEEDMOREPARAMS("OPER") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NEEDMOREPARAMS("OPER"));
		return;
	} else if (arg.size() > 2) {
		std::cerr << reply_.ERR_TOOMANYPARAMS("OPER") << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_TOOMANYPARAMS("OPER"));
		return;
	} else if (!this->server_.isUser(arg.at(0))) {
		std::cerr << reply_.ERR_NOSUCHNICK(arg.at(0)) << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_NOSUCHNICK(arg.at(0)));
		return;
	} else if (this->server_.getOperPass() != arg.at(1)) {
		std::cerr << reply_.ERR_PASSWDMISMATCH() << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  reply_.ERR_PASSWDMISMATCH());
		return;
	}
	const User &oper_user = this->server_.getUser(arg.at(0));
	if (oper_user.hasMode(User::o)) {
		std::cerr << oper_user.getNickName() << " is already an IRC operator"
				  << std::endl;
		this->server_.sendMsgToClient(user.getFd(),
									  oper_user.getNickName() +
										  " is already an IRC operator");
		return;
	}
	const_cast<User &>(oper_user).setMode(User::o);
	std::cout << reply_.RPL_YOUREOPER() << std::endl;
	if (user.getFd() != oper_user.getFd()) {
		this->server_.sendMsgToClient(oper_user.getFd(),
									  reply_.RPL_YOUREOPER());
	}
	this->server_.sendMsgToClient(user.getFd(), oper_user.getNickName() +
													" is now an IRC operator");
}
