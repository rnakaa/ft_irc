#include "Server.hpp"

Server::Server(const int argc, const char **argv) {
	try {
		checkValidArgc(argc);
		checkValidPort(argv[1]);
		setPortAndPass(argv);
	} catch (const std::exception& e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void Server::checkValidArgc(const int argc) const {
	if (argc != 3) {
		throw std::invalid_argument("Invalid argument: Usage \"./ircserv <port> <password>\"");
	}
}

void Server::checkValidPort(const char *str) const {
	std::istringstream iss(str);
	long port;
	iss >> port;
	if (iss.fail() || port < 1024 || port > 65535) {
		throw std::invalid_argument("Invalid argument: Port number must be between 1024 and 65535");
	}
}

void Server::setPortAndPass(const char **argv) {
	std::istringstream iss_port(argv[1]), iss_pass(argv[2]);
	iss_port >> this->port_;
	if (iss_port.fail()) {
		throw std::invalid_argument("Invalid argument: Invalid port format provided");
	}
	iss_pass >> this->pass_;
	if (iss_pass.fail()) {
		throw std::invalid_argument("Invalid argument: Invalid password format provided");
	}
}

void Server::init() {
}

void Server::run() {
}

Server::~Server() {
}

