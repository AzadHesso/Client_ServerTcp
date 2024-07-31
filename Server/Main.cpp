#include "Server.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if(argc < 2) {
        std::cerr << "Usage: Server <port> " << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    Server server(port);
    server.run();

    return 0;
}
