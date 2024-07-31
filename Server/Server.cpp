#include "Server.h"
#include <iostream>
#include <thread>
#include <fstream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdexcept>
#include <mutex>

Server::Server(int port) : _port(port), _serverSocket(0) {}

void Server::run() {
    _serverSocket = socket(AF_INET, SOCK_STREAM, 0);   
    if(_serverSocket < 0) {
        throw std::runtime_error("Error opening socket");
    }

    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(_port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(_serverSocket, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Error binding socket");
    }

    if(listen(_serverSocket, 5) < 0) {
        close(_serverSocket);
        throw std::runtime_error("Error listening on socket");
    }

    std::cout << "Server is running on port " << _port << std::endl;

    while(true) {
        int clientSocket = accept(_serverSocket, NULL, NULL);
        if (clientSocket < 0) {
            std::cerr << "Error accepting connection" << std::endl;
            continue;
        }

        std::thread(&Server::handleClient, this, clientSocket).detach();
    }
}

void Server::handleClient(int clientSocket) {
    try {
        char buffer[1024] = {0};
        int bytesRead = read(clientSocket, buffer, sizeof(buffer));
        if (bytesRead < 0) {
            close(clientSocket);
            throw std::runtime_error("Error reading from socket");
        }

        if (bytesRead > 0) {
            std::lock_guard<std::mutex> guard(logMutex); 
            std::ofstream logFile("log.txt", std::ios::app);
            if (!logFile.is_open()) {
                throw std::runtime_error("Error opening log file");
            }
            logFile << buffer << std::endl;
        }
    } catch (const std::exception &e) {
        std::cerr << "Exception in handleClient: " << e.what() << std::endl;
    }
    close(clientSocket);
}
