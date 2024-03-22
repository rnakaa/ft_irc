#include "Command.hpp"

Command::Command(Server &server) : server_(server) {
	this->commands_map_["TEST"] = &Command::TEST;
	this->commands_map_["PASS"] = &Command::PASS;
	// this->commands_map_["MOD"] = &Command::MOD;
	this->commands_map_["JOIN"] = &Command::JOIN;
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

void Command::TEST(User &user, std::vector<std::string> &arg) {
	(void)arg;
	std::cout << "Command => printTest " << std::endl;
	this->server_.printChannelName();
	server_.sendMsgToClient(user.getFd(), "Command => printTest: Hello world!");
}

// void Command::MOD(User &user, std::vector<std::string> &arg) {
//	(void)arg;
//	user.setMode(User::s);
//	std::cout << user.getMode() << std::endl;
//	std::cout << user.hasMode(User::s) << std::endl;
//	std::cout << "check w" << std::endl;
//	std::cout << user.hasMode(User::w) << std::endl;
//	std::cout << "+w" << std::endl;
//	user.setMode(User::w);
//	std::cout << "check w" << std::endl;
//	std::cout << user.hasMode(User::w) << std::endl;
//	std::cout << "check o" << std::endl;
//	std::cout << user.hasMode(User::o) << std::endl;
//	std::cout << "check a" << std::endl;
//	std::cout << user.hasMode(User::a) << std::endl;
//	std::cout << "+a" << std::endl;
//	user.setMode(User::a);
//	std::cout << user.hasMode(User::a) << std::endl;
// }
