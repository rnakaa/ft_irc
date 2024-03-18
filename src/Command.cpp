#include "Command.hpp"

Command::Command(Server &server) : server_(server) {
	this->commands_map_["PASS"] = Command::pass;
	std::cout << "server pass is" << server_.getPass() << std::endl;
}

void Command::executeCommand(const int fd) {
	CommandFunction func = this->commands_map_[this->command_name_];
	if (func) {
		func(fd, this->arg_);
	} else {
		std::cout << "Unknown Command: " << this->command_name_ << std::endl;
	}
}

void Command::pass(const int fd, std::vector<std::string> &arg) {
	(void)fd;
	try {
		arg.at(0);
		std::cout << "pass is " << arg.at(0) << std::endl;
	} catch (const std::out_of_range &e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}

void Command::parseClientMessage(std::string &message) {
	std::istringstream iss(message);
	std::string word;

	iss >> this->command_name_;
	while (iss >> word) {
		this->arg_.push_back(word);
	}
}

void Command::handleCommand(const int fd, std::string &message) {
	parseClientMessage(message);
	executeCommand(fd);
}
