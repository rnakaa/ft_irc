#include "Command.hpp"

Command::Command(Server &server) : server_(server) {
	this->commands_map_["TEST"] = &Command::TEST;
	this->commands_map_["PASS"] = &Command::PASS;
	// std::cout << "server pass is" << server_.getPass() << std::endl;
}

void Command::handleCommand(User &user, std::string &message) {
	// std::cout << "start handleCommand" << std::endl;
	try {
		parseClientMessage(message);
		CommandFunction func = this->commands_map_[this->command_name_];
		if (!func) {
			throw std::runtime_error("ERROR: Unknown Command: " +
									 this->command_name_);
		}
		(this->*func)(user, this->arg_);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		this->server_.sendMsgToClient(user.getFd(), e.what());
	}
}

void Command::parseClientMessage(const std::string &message) {
	// std::cout << "start parseClientMessage" << std::endl;
	std::istringstream iss(message);
	std::string str;

	iss >> this->command_name_;
	if (iss.fail()) {
		throw std::runtime_error(
			"ERROR: cannot parse command name received from client");
	}
	while (iss >> str) {
		if (iss.fail()) {
			throw std::runtime_error(
				"ERROR: cannot parse command arguments received from client");
		}
		this->arg_.push_back(str);
	}
}

void Command::PASS(User &user, std::vector<std::string> &arg) {
	std::cout << "start pass " << user.getFd() << std::endl;
	if (user.getAuthFlags() != User::NONE_AUTH) {
		std::cerr << "ERR_ALREADYREGISTRED" << std::endl;
		server_.sendMsgToClient(user.getFd(), "ERR_ALREADYREGISTRED");
	} else if (arg.empty()) {
		std::cerr << "ERR_NEEDMOREPARAMS" << std::endl;
		server_.sendMsgToClient(user.getFd(), "ERR_NEEDMOREPARAMS");
	} else if (this->server_.getPass() != arg.at(0)) {
		std::cerr << "ERR_PASSNOTCORRECT" << std::endl;
		server_.sendMsgToClient(user.getFd(), "ERR_PASSNOTCORRECT");
	} else {
		user.setAuthFrags(User::PASS_AUTH);
		std::cout << "SUCCESS: PASS Command client[" << user.getFd()
				  << "], auth_flag_: " << user.getAuthFlags() << std::endl;
		this->server_.sendMsgToClient(user.getFd(), "SUCCESS: PASS Command");
	}
}

void Command::TEST(User &user, std::vector<std::string> &arg) {
	(void)arg;
	std::cout << "Command => printTest " << std::endl;
	server_.sendMsgToClient(user.getFd(), "Command => printTest: Hello world!");
}
