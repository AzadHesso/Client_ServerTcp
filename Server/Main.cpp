#include "Server.h"
#include <iostream>
#include <exception>

int main(int argc, char *argv[]) {
    try {
        if(argc < 2) {
            std::cerr << "Usage: Server <port>" << std::endl;
            return 1;
        }

        int port = std::stoi(argv[1]);
        Server server(port);
        server.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception caught in main: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
