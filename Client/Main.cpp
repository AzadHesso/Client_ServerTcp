#include "Client.h"
#include <iostream>

int main(int argc, char *argv[]) {
    if(argc < 4) {
        std::cerr << "Usage: client <name> <port> <period>" << std::endl;
        return 1;
    }

    std::string name = argv[1];
    int port = std::stoi(argv[2]);
    int period = std::stoi(argv[3]);
    std::string serverIp = "127.0.0.1";

    Client client(name, serverIp, port, period);
    client.run();

    return 0;
}
