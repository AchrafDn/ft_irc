#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>

#include "Client.hpp"

class Server
{
private:
    int port;
    std::string password;
    std::string serverName;
    std::string creationTime;
    int fd_SerSocket;

    std::vector<struct pollfd> fds;
    std::map<int, Client> clients;

public:
    Server(int Port, std::string Password) : port(Port), password(Password) {}

    std::string get_servername() { return serverName; }
    void set_time(std::string time) { creationTime = time; }

    void execution();
    void SerSocket();
    void AcceptClient();
    void ReceiveData(int fd);
    void CreateClient(int fd);
    void RemoveClient(int fd);
    void RegisterClient(std::string input, int fd);
    void SendMessage(int fd, std::string message);

    /*COMMANDS*/
    void pass_cmd(std::istringstream &iss, int fd);
    void handle_pong(int fd);
};

#endif