#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <iostream>
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
#include <sstream>

#include "Client.hpp"
#include "Channel.hpp"

// added forward declaration
class Channel;
class Client;

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
    //added channel vector
    std::vector<Channel *> channels;

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
    // added channel methods
    void AddChannel(Channel *channel);
    void RemoveChannel(Channel *channel);
    void CreateChannel(std::vector<std::string> command, Client *creator);

    // command methods
    
    void Join(std::vector<std::string> command, Client *client);
    void Mode(std::vector<std::string> command, Client *client);
};

#endif