#include "Command.hpp"

void Command::PASS(User &user, std::vector<std::string> &arg) {
	std::cout << "start pass " << user.getFd() << std::endl;
	if (user.getAuthFlags() == User::PASS_AUTH) {
		std::cerr << error_.ERR_ALREADYREGISTRED() << std::endl;
		server_.sendMsgToClient(user.getFd(), error_.ERR_ALREADYREGISTRED());
	} else if (arg.empty()) {
		std::cerr << error_.ERR_NEEDMOREPARAMS("PASS") << std::endl;
		server_.sendMsgToClient(user.getFd(),
								error_.ERR_NEEDMOREPARAMS("PASS"));
	} else if (this->server_.getPass() != arg.at(0)) {
		std::cerr << error_.ERR_PASSWDMISMATCH() << std::endl;
		server_.sendMsgToClient(user.getFd(), error_.ERR_PASSWDMISMATCH());
	} else {
		user.setAuthFrags(User::PASS_AUTH);
		std::cout << "SUCCESS: PASS Command client[" << user.getFd()
				  << "], auth_flag_: " << user.getAuthFlags() << std::endl;
		this->server_.sendMsgToClient(user.getFd(), "SUCCESS: PASS Command");
	}
}
