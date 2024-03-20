#include <sys/socket.h>
#include <iostream>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib> //exit
#include <cstring> //strlen

#define PORT 6667
#define SERVER_IP "127.0.0.1"
#define BUF_SIZE 1024

void	exit_error(const std::string& msg) {
	std::cerr << "ERROR: " << msg << std::endl;
	std::exit(1);
}

int main() {
	int client_sockfd;

	client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (client_sockfd == -1) {
		exit_error("socket");
	}
	std::cout << "SUCCESS: socket: " << client_sockfd << std::endl;

	struct sockaddr_in client_addr;
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(PORT);
	client_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

	if (connect(client_sockfd, (struct sockaddr *)&client_addr, sizeof(client_addr)) == -1) {
		close(client_sockfd);
		exit_error("connect");
	}

	std::cout << "SUCCESS: connection" << std::endl;

	while (1) {
		std::cout << "input message: ";
		//std::string send_msg;
		char send_msg[BUF_SIZE];
		std::cin.getline(send_msg,BUF_SIZE);

		int send_size = send(client_sockfd, &send_msg, std::strlen(send_msg), 0);
		if (send_size == -1) {
			std::cerr << "ERROR: send" << std::endl;
			break ;
		}
		
		char recv_msg[BUF_SIZE] = {0};
		ssize_t recv_size = recv(client_sockfd, &recv_msg, BUF_SIZE, 0);
		if (recv_size == -1) {
			std::cerr << "ERROR: recv" << std::endl;
			break ;
		} else if (recv_size == 0) {
			std::cout << "finish connection" << std::endl;
			break ;
		}
		std::cout << recv_msg;
	}

	close(client_sockfd);
	return (0);
}
