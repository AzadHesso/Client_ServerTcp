#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <mutex>


class Client {
public:    

    Client(const std::string& name, const std::string& serverIp, int port, int period);
    void run();

private:
    void sendMessages();
    std::string formatCurrentTime();

    std::string _name;
    std::string _serverIp;
    int _port;
    int _period;

    std::mutex _mutex;
};

#endif 