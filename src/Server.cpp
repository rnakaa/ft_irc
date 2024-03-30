#include "Server.hpp"

Server::Server(const int argc, const char **argv) : oper_pass_("password") {
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
	this->server_addr_.sin_port = htons(this->port_);
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

	struct pollfd server_pollfd;
	server_pollfd.fd = this->server_sockfd_;
	server_pollfd.events = POLLIN;
	server_pollfd.revents = 0;
	this->pollfd_vec_.push_back(server_pollfd);
}

void Server::run() {
	while (1) {
		int ret = poll(this->pollfd_vec_.data(), this->pollfd_vec_.size(),
					   POLL_WAIT_FOREVER);
		if (ret == -1) {
			close(this->server_sockfd_);
			exit_error("poll", strerror(errno));
		}
		handlPollEvents();
	}
	close(this->server_sockfd_);
}

void Server::handlPollEvents() {
	for (size_t i = 0; i < this->pollfd_vec_.size(); ++i) {
		if (this->pollfd_vec_[i].revents & POLLIN) {
			if (this->pollfd_vec_[i].fd == this->server_sockfd_) {
				acceptNewClientConnect();
			} else {
				try {
					this->recv_msg_ = recvCmdFromClient(i);
					std::cout << "client[" << this->pollfd_vec_[i].fd << "]: \""
							  << this->recv_msg_ << "\"" << std::endl;
					if (this->user_map_.find(this->pollfd_vec_[i].fd) ==
						this->user_map_.end()) {
						std::cout << "Warning: fd " << this->pollfd_vec_[i].fd
								  << " not found in user_map_" << std::endl;
					}
					std::istringstream iss(recv_msg_);

					while (std::getline(iss, this->recv_msg_)) {
						Command cmd(*this);
						cmd.handleCommand(
							this->user_map_[this->pollfd_vec_[i].fd],
							this->recv_msg_);
					}
				} catch (const std::exception &e) {
					continue;
				}
			}
		}
	}
}

void Server::acceptNewClientConnect() {
	socklen_t client_addr_len = sizeof(this->client_addr_);
	struct pollfd client_pollfd;
	client_pollfd.fd =
		accept(this->server_sockfd_,
			   reinterpret_cast<struct sockaddr *>(&this->client_addr_),
			   &client_addr_len);
	if (client_pollfd.fd == -1) {
		close(this->server_sockfd_);
		exit_error("accept", strerror(errno));
	}
	std::cout << "SUCCESS: connection from client[" << client_pollfd.fd << "]"
			  << std::endl;
	client_pollfd.events = POLLIN;
	client_pollfd.revents = 0;
	User client_user(client_pollfd.fd);
	this->user_map_.insert(std::make_pair(client_pollfd.fd, client_user));
	this->pollfd_vec_.push_back(client_pollfd);
}

std::string Server::recvCmdFromClient(const size_t i) {
	char recv_msg[BUF_SIZE] = {0};
	ssize_t recv_size =
		recv(this->pollfd_vec_[i].fd, &recv_msg, BUF_SIZE, MSG_DONTWAIT);
	if (recv_size > MAX_MSG_LEN) {
		std::cout << "ERROR: received message exceeds 510 characters limit"
				  << std::endl;
		return ("ERROR: received message exceeds 510 characters limit");
	} else if (recv_size == -1) {
		close(this->pollfd_vec_[i].fd);
		close(this->server_sockfd_);
		exit_error("recv", strerror(errno));
	} else if (recv_size == 0) {
		std::cout << "finish connection from client[" << this->pollfd_vec_[i].fd
				  << "]" << std::endl;
		Command cmd(*this);
		User &user = user_map_[this->pollfd_vec_[i].fd];
		const int fd = user.getFd();
		cmd.quitAllChannels(user,
							"finish connection from " + user.getNickName());
		close(fd);
		this->user_map_.erase(fd);
		this->pollfd_vec_.erase(this->pollfd_vec_.begin() + i);
		throw std::runtime_error("finish connection from client");
	}
	return (recv_msg);
}

void Server::sendMsgToClient(const int fd, const std::string &send_str) const {
	// std::cout << "start sendMsgToClient" << std::endl;
	char send_msg[BUF_SIZE];
	if (send_str.size() + 1 > BUF_SIZE - 1) {
		std::strcpy(send_msg, "ERROR : send message too long");
	} else {
		std::strcpy(send_msg, send_str.c_str());
	}
	std::strcat(send_msg, "\n");
	int send_size = send(fd, &send_msg, std::strlen(send_msg), MSG_DONTWAIT);
	if (send_size == -1) {
		exit_error("send", strerror(errno));
	}
}

void Server::removeChannel(const std::string &ch_name) {
	this->ch_map_.erase(ch_name);
	std::cout << "remove Channel: " << ch_name << std::endl;
}

void Server::removeUser(const int fd) { this->user_map_.erase(fd); }

void Server::removePollfd(const int fd) {
	for (size_t i = 0; i < this->pollfd_vec_.size(); ++i) {
		if (this->pollfd_vec_.at(i).fd == fd) {
			this->pollfd_vec_.erase(pollfd_vec_.begin() + i);
		}
	}
}

void Server::exit_error(const std::string &func,
						const std::string &err_msg) const {
	std::cerr << "ERROR: " << func << ": " << err_msg << std::endl;
	std::exit(EXIT_FAILURE);
}

const std::string &Server::getPass() const { return (this->pass_); }

const Channel &Server::getChannel(const std::string &ch_name) const {
	return ch_map_.find(ch_name)->second;
}

const User &Server::getUser(const std::string &nickname) const {
	for (std::map<int, User>::const_iterator it = user_map_.begin();
		 it != user_map_.end(); ++it) {
		if (nickname == it->second.getNickName()) {
			return it->second;
		}
	}
	throw std::runtime_error("cannot find user");
}

const std::string &Server::getUserNickName(const int user_fd) const {
	return user_map_.find(user_fd)->second.getNickName();
}

const std::string &Server::getOperPass() const { return this->oper_pass_; }

void Server::setChannel(const std::string &ch_name, const Channel &ch) {
	this->ch_map_.insert(std::make_pair(ch_name, ch));
}

bool Server::hasChannelName(const std::string &ch_name) const {
	return ch_map_.find(ch_name) != ch_map_.end();
}

void Server::printChannelName() const {
	for (std::map<std::string, Channel>::const_iterator it = ch_map_.begin();
		 it != ch_map_.end(); ++it) {
		std::cout << "Channel Name: " << it->first
				  << ", Channel: " << it->second.getName() << std::endl;
	}
}

bool Server::nicknameExist(const std::string &nickname) const {
	return this->nickname_log_.find(nickname) != this->nickname_log_.end();
}

void Server::nicknameInsertLog(std::string nickname) {
	this->nickname_log_.insert(nickname);
}

void Server::sendToChannelUser(const std::string &ch_name,
							   const std::string &msg) const {
	std::cout << "start sendToChannelUser" << std::endl;
	if (!hasChannelName(ch_name))
		return;
	const Channel &ch = getChannel(ch_name);
	std::map<int, User *>::const_iterator iter =
		const_cast<Channel &>(ch).getMapBeginIterator();
	while (iter != const_cast<Channel &>(ch).getMapEndIterator()) {
		sendMsgToClient(iter->second->getFd(), msg);
		++iter;
	}
}

void Server::sendToChannelUser(const std::string &ch_name, const User &user,
							   const std::string &msg) const {
	std::cout << "start sendToChannelUser" << std::endl;
	if (!hasChannelName(ch_name))
		return;
	const std::string send_msg = ":" + user.getNickName() + "!" +
								 user.getUserName() + "ft_ircserver" +
								 " PRIVMSG " + ch_name + " :" + msg;
	const Channel &ch = getChannel(ch_name);
	std::map<int, User *>::const_iterator iter =
		const_cast<Channel &>(ch).getMapBeginIterator();
	while (iter != const_cast<Channel &>(ch).getMapEndIterator()) {
		if (user.getFd() == iter->second->getFd()) {
			++iter;
			continue;
		}
		sendMsgToClient(iter->second->getFd(), send_msg);
		++iter;
	}
}

bool Server::isUser(const std::string &nickname) const {
	for (std::map<int, User>::const_iterator it = user_map_.begin();
		 it != user_map_.end(); ++it) {
		if (nickname == it->second.getNickName()) {
			return true;
		}
	}
	return false;
}

Server::~Server() {}
