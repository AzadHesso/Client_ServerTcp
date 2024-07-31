#ifndef SERVER_H
#define SERVER_H

#include <string>
#include <mutex>

class Server {
public:
    Server(int port);
    void run();

private:
    int _port;
    int _serverSocket;
    std::string logFileName;

    std::mutex logMutex;

    void handleClient(int client_socket);
};

#endif
