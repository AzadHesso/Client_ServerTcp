#include "Client.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <ctime>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

Client::Client(const std::string& name, const std::string& serverIp, int port, int period)
 : _name(name), _serverIp(serverIp), _port(port), _period(period) {}

void Client::run() {
    std::thread sendThread(&Client::sendMessages, this);
    sendThread.join();
}

void Client::sendMessages() {
    while(true) {
        int socket_file_descriptor = socket(AF_INET,SOCK_STREAM,0);
        if(socket_file_descriptor < 0) {
            std::cerr << "Error opening socket!" << std::endl;
            return;
        }

        sockaddr_in serv_addr;
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(_port);
        inet_pton(AF_INET, _serverIp.c_str() , &serv_addr.sin_addr);

        if(connect(socket_file_descriptor,(sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            std::cerr << "Error connecting to server" << std::endl;
            close(socket_file_descriptor);
            return;
        }


        std::lock_guard<std::mutex> guard(_mutex);
        std::string message = formatCurrentTime() + " " + _name;

        write(socket_file_descriptor, message.c_str(), message.length());
        close(socket_file_descriptor);

        std::this_thread::sleep_for(std::chrono::seconds(_period));

    }
}

std::string Client::formatCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto now_time_t = std::chrono::system_clock::to_time_t(now);
    auto now_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;

    std::tm now_tm = *std::localtime(&now_time_t);

    char timestamp[100];
    std::strftime(timestamp, sizeof(timestamp), "[%Y-%m-%d %H:%M:%S", &now_tm);
    return std::string(timestamp) + "." + std::to_string(now_ms.count()) + "]";
}

