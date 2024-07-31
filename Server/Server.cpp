#include "Server.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

Server::Server(int port) : _port(port), _serverSocket(0) {}

void Server::run() {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);   
    if(_serverSocket < 0) {
        std::cerr << "Error opening socket" << std::endl;
        return;
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(_serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error binding socket" << std::endl;
        close(_serverSocket);
        return;
    }

    if(listen(_serverSocket, 5) < 0) {
        std::cerr << "Error listening on socket" << std::endl;
        close(_serverSocket);
        return;
    }

    std::cout << "Server is running on port " << _port << std::endl;

    while(true){
        int clientSocket = accept(_serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::thread(&Server::handleClient, this, clientSocket).detach();
    }
}

void Server::handleClient(int clientSocket) {
    char buffer[1024] = {0};
    int bytesRead = read(clientSocket, buffer, sizeof(buffer));
    if(bytesRead > 0) {
        std::ofstream logFile("log.txt", std::ios::app);
        logFile << buffer << std::endl;
    }
    close(clientSocket);
}
