#include "Server.hpp"

Server::Server(const int argc, const char **argv) {
	try {
		checkValidArgc(argc);
		checkValidPort(argv[1]);
		setPortAndPass(argv);
	} catch (const std::exception &e) {
		std::cerr << "ERROR: " << e.what() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

void Server::checkValidArgc(const int argc) const {
	if (argc != 3) {
		throw std::invalid_argument(
			"Invalid argument: Usage \"./ircserv <port> <password>\"");
	}
}

void Server::checkValidPort(const char *str) const {
	std::istringstream iss(str);
	long port;
	iss >> port;
	if (iss.fail() || port < 1024 || port > 65535) {
		throw std::invalid_argument(
			"Invalid argument: Port number must be between 1024 and 65535");
	}
}

void Server::setPortAndPass(const char **argv) {
	std::istringstream iss_port(argv[1]), iss_pass(argv[2]);
	iss_port >> this->port_;
	if (iss_port.fail()) {
		throw std::invalid_argument(
			"Invalid argument: Invalid port format provided");
	}
	iss_pass >> this->pass_;
	if (iss_pass.fail()) {
		throw std::invalid_argument(
			"Invalid argument: Invalid password format provided");
	}
}

void Server::init() {
	this->server_sockfd_ = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_sockfd_ == -1) {
		exit_error("socket", strerror(errno));
	}
	std::cout << "SUCCESS: socket: " << this->server_sockfd_ << std::endl;

	this->server_addr_.sin_family = AF_INET;
	this->server_addr_.sin_port = htons(PORT);
	this->server_addr_.sin_addr.s_addr = inet_addr(SERVER_IP);
	if (bind(this->server_sockfd_,
			 reinterpret_cast<struct sockaddr *>(&this->server_addr_),
			 sizeof(this->server_addr_)) < 0) {
		close(this->server_sockfd_);
		exit_error("bind", strerror(errno));
	}
	std::cout << "SUCCESS: bind" << std::endl;

	if (listen(this->server_sockfd_, SOMAXCONN)) {
		close(this->server_sockfd_);
		exit_error("listen", strerror(errno));
	}
	std::cout << "SUCCESS: listen" << std::endl;
}

void Server::run() {}

void Server::exit_error(const std::string &func, const std::string &err_msg) {
	std::cerr << "ERROR: " << func << ": " << err_msg << std::endl;
	std::exit(EXIT_FAILURE);
}

Server::~Server() {}
