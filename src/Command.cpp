#include "Command.hpp"

Command::Command(Server &server) : server_(server) {
	this->commands_map_["TEST"] = &Command::TEST;
	this->commands_map_["PASS"] = &Command::PASS;
	this->commands_map_["NICK"] = &Command::NICK;
	this->commands_map_["USER"] = &Command::USER;
	this->commands_map_["QUIT"] = &Command::QUIT;
	this->commands_map_["JOIN"] = &Command::JOIN;
	this->commands_map_["MODE"] = &Command::MODE;
	this->commands_map_["PRIVMSG"] = &Command::PRIVMSG;
	this->commands_map_["INVITE"] = &Command::INVITE;
	this->commands_map_["TOPIC"] = &Command::TOPIC;
	this->commands_map_["OPER"] = &Command::OPER;
	this->commands_map_["KILL"] = &Command::KILL;
	this->commands_map_["PING"] = &Command::PING;
	this->mode_map_['O'] = &Command::handleChannelOriginOperator;
	this->mode_map_['o'] = &Command::handleChannelOperator;
	this->mode_map_['k'] = &Command::handleChannelKey;
	this->mode_map_['l'] = &Command::handleLimitedUserNum;
	this->mode_map_['l'] = &Command::handleLimitedUserNum;
	this->mode_map_['i'] = &Command::handleInviteOnly;
	this->mode_map_['t'] = &Command::handleTopicOnlyOperator;
	// std::cout << "server pass is" << server_.getPass() << std::endl;
}

void Command::handleCommand(User &user, std::string &message) {
	// std::cout << "start handleCommand" << std::endl;
	try {
		this->recv_message_ = message;
		std::cout << "comand handl " << message << std::endl;
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
