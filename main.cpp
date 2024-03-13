#include "Server.hpp"

void	exit_error(const std::string& msg) {
	std::cerr << "ERROR: " << msg << std::endl;
	std::exit(1);
}

int main(const int argc, const char **argv) {
	Server server(argc, argv);
	server.init();
	server.run();
}

// int main () {
// 	int server_sockfd;
// 
// 	server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
// 	if (server_sockfd == -1) {
// 		exit_error("socket");
// 	}
// 	std::cout << "SUCCESS: socket: " << server_sockfd << std::endl;
// 
// 	struct sockaddr_in server_addr;
// 	server_addr.sin_family = AF_INET;
// 	server_addr.sin_port = htons(PORT);
// 	server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
// 	if (bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
// 		close(server_sockfd);
// 		exit_error("bind");
// 	}
// 	std::cout << "SUCCESS: bind" << std::endl;
// 
// 	if (listen(server_sockfd, SOMAXCONN)) {
// 		close(server_sockfd);
// 		exit_error("listen");
// 	}
// 	std::cout << "SUCCESS: listen" << std::endl;
// 
// 	while (1) {
// 		struct sockaddr_in client_addr;
// 		socklen_t client_addr_len = sizeof(client_addr);
// 		int client_sockfd = accept(server_sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
// 		if (client_sockfd == -1) {
// 			close(server_sockfd);
// 			exit_error("accept");
// 		}
// 		std::cout << "SUCCESS: connection: " << client_sockfd << std::endl;
// 
// 		while (1) {
// 			char recv_msg[BUF_SIZE] = {0};
// 			ssize_t recv_size = recv(client_sockfd, &recv_msg, BUF_SIZE, 0);
// 			if (recv_size == -1) {
// 				close(client_sockfd);
// 				close(server_sockfd);
// 				exit_error("recv");
// 			} else if (recv_size == 0) {
// 				std::cout << "finish connection" << std::endl;
// 				break ;
// 			}
// 
// 			std::cout << "message from client: \"" << recv_msg << "\"" << std::endl;
// 
// 			// char send_msg[BUF_SIZE] = "server received your message";
// 			int send_size = send(client_sockfd, &recv_msg, std::strlen(recv_msg), 0);
// 			if (send_size == -1) {
// 				std::cerr << "ERROR: send" << std::endl;
// 				break ;
// 			}
// 		}
// 		close(client_sockfd);
// 	}
// 	close(server_sockfd);
// 	return 0;
// }
