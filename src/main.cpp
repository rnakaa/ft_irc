#include "Server.hpp"

int main(const int argc, const char **argv) {
	Server server(argc, argv);
	server.init();
	server.run();
}
