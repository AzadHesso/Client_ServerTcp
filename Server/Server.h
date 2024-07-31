#ifndef SERVER_H
#define SERVER_H

#include <string>

class Server {
public:
    Server(int port);
    void run();

private:
    int _port;
    int _serverSocket;
    std::string logFileName;

    void handleClient(int client_socket);
};

#endif
