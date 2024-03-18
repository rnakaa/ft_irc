#include "Command.hpp"

Command::Command(Server &server) : server_(server) {
	this->commands_map_["PASS"] = &Command::pass;
	this->commands_map_["TEST"] = &Command::printTest;
	// std::cout << "server pass is" << server_.getPass() << std::endl;
}

void Command::handleCommand(const int fd, std::string &message) {
	// std::cout << "start handleCommand" << std::endl;
	try {
		parseClientMessage(message);
		executeCommand(fd);
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		server_.sendMsgToClient(fd, e.what());
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

void Command::executeCommand(const int fd) {
	// std::cout << "start executeCommand" << std::endl;
	CommandFunction func = this->commands_map_[this->command_name_];
	if (func) {
		(this->*func)(fd, this->arg_);
	} else {
		throw std::runtime_error("ERROR: Unknown Command: " +
								 this->command_name_);
	}
}

void Command::pass(const int fd, std::vector<std::string> &arg) {
	std::cout << "start pass " << fd << std::endl;
	try {
		std::cout << "pass is " << arg.at(0) << std::endl;
	} catch (const std::out_of_range &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void Command::printTest(const int fd, std::vector<std::string> &arg) {
	(void)arg;
	std::cout << "Command => printTest " << std::endl;
	server_.sendMsgToClient(fd, "Command => printTest: Hello world!");
}
