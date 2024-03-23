#include "Server.hpp"
#include "Client.hpp"
#include "cstdio"
#include <sstream>

void Server::CreateClient(int fd)
{
    Client *a = new Client(fd, 0);
    clients.insert(std::pair<int, Client>(fd, *a));

    struct pollfd fd_poll;

    fd_poll.fd = fd;
    fd_poll.events = POLLIN;
    fd_poll.revents = 0;

    fds.push_back(fd_poll);

    std::cout << "Creation of client finished" << std::endl;
    SendMessage(fd, "Please enter your password: \r\n");
}

void Server::AcceptClient()
{
    struct sockaddr_in addr;

    addr.sin_port = htons(this->port);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t len = sizeof(addr);

    int new_fd = accept(fd_SerSocket, (struct sockaddr *)&addr, &len);
    if (new_fd == -1)
    {
        perror("accept");
        throw std::string("accept client failed");
    }

    CreateClient(new_fd);
    std::cout << "client " << new_fd << " accepted" << std::endl;
}

void Server::SerSocket()
{
    this->fd_SerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (this->fd_SerSocket == -1)
        throw std::string("Server's socket creation failed");

    int en = 1;
    if (setsockopt(fd_SerSocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
        throw std::string("setsockopt() failed");

    // if (fcntl(fd_SerSocket, F_SETFL, O_NONBLOCK) == -1)
    //     throw std::string("fcnt() failed");
    struct sockaddr_in addr;

    addr.sin_port = htons(this->port);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(fd_SerSocket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        throw std::string("Binding server's socket failed");
    if (listen(fd_SerSocket, SOMAXCONN) == -1)
        throw std::string("Listening to incoming client requests");

    struct pollfd fd_poll;

    fd_poll.fd = fd_SerSocket;
    fd_poll.events = POLLIN;
    fd_poll.revents = 0;

    fds.push_back(fd_poll);
}

void Server::RemoveClient(int fd)
{
    close(fd);
    clients.erase(fd);
    for (std::vector<struct pollfd>::iterator it = this->fds.begin(); it != fds.end(); it++)
    {
        if (it->fd == fd)
        {
            fds.erase(it);
            break;
        }
    }
}

void Server::SendMessage(int fd, std::string message)
{
    send(fd, message.c_str(), message.size(), 0);
}

void Server::pass_cmd(std::istringstream &iss, int fd)
{
    std::string input;
    iss >> input;
    if (iss.eof())
        SendMessage(fd, "Client PASS :Not enough parameters\r\n");
    else if (password == input)
    {
        iss >> input;
        if (!iss.eof() || input != password)
        {
            SendMessage(fd, "Client :Password incorrect\r\n");
        }
        else
        {
            clients[fd].set_registered(1);
            SendMessage(fd, "Please enter your NICK:\r\n");
        }
    }
    else
        SendMessage(fd, "Client :Password incorrect\r\n");
}

void Server::RegisterClient(std::string buffer, int fd)
{
    std::istringstream iss((std::string(buffer)));
    std::string input;
    std::string message;
    iss >> input;
    if (input == "PASS")
    {
        pass_cmd(iss, fd);
    }
    else if (clients[fd].get_registered() == 1 && input == "NICK")
    {
        iss >> input;
        clients[fd].set_nick(input);
        clients[fd].set_registered(2);
        message = "Please enter your USER:\r\n";
        send(fd, message.c_str(), message.size(), 0);
    }
    else if (clients[fd].get_registered() == 2 && input == "USER")
    {
        iss >> input;
        clients[fd].set_user(input);

        iss >> input;
        iss >> input;

        iss >> input;
        clients[fd].set_real_name(input);
        clients[fd].set_registered(3);

        message = ":HALAL_TINDER 001 " + clients[fd].get_nick() + " :Welcome to the HALAL_TINDER Network, " + clients[fd].get_nick() + "[!" + clients[fd].get_user() + "@tinderhalal.com]\r\n";

        send(fd, message.c_str(), message.size(), 0);

        message = ":HALAL_TINDER 002 " + clients[fd].get_nick() + " :Your host is HALAL_TINDER, running version 2.0\r\n";

        send(fd, message.c_str(), message.size(), 0);

        message = ":HALAL_TINDER 003 " + clients[fd].get_nick() + " :This server was created " + creationTime + "\r\n";

        send(fd, message.c_str(), message.size(), 0);

        message = ":HALAL_TINDER 004 " + clients[fd].get_nick() + " :HALAL_TINDER 2.0 \r\n";

        send(fd, message.c_str(), message.size(), 0);
    }
}

void Server::ReceiveData(int fd)
{
    char buffer[1000];

    int bytes = recv(fd, buffer, sizeof(buffer) - 1, 0);
    buffer[bytes] = '\0';
    if (bytes <= 0)
    {
        if (bytes == -1)
            throw std::string("read() failled reading data received from client");
        else
            RemoveClient(fd);
        std::cout << "Client " << fd << " Disconnected" << std::endl;
    }
    else
    {
        std::vector<std::string> cmd;
        std::istringstream iss(buffer);
        std::string input;
        while (iss >> input)
            cmd.push_back(input);
        if (std::string(buffer).find("PING"))
        {
            std::string message = "PONG HALLAL_TINDER\r\n";
            send(fd, message.c_str(), message.size(), 0);
        }
        if (clients[fd].get_registered() < 3)
            RegisterClient(std::string(buffer), fd);
        else
            std::cout << buffer;
    }
}

void Server::handle_pong(int fd)
{
    std::string sender = clients[fd].get_name();
    std::string message = "PONG";
    SendMessage(fd, message);
}

void Server::execution()
{

    SerSocket();
    std::cout << "creation of server socket is done wainting for inconming client connection" << std::endl;

    while (true)
    {
        if (poll(&fds[0], fds.size(), -1) == -1)
            throw std::string("poll() failed");
        for (size_t i = 0; i < fds.size(); i++)
        {
            if (fds[i].revents & POLLIN)
            {
                if (fds[i].fd == fd_SerSocket)
                    AcceptClient();
                else
                    ReceiveData(fds[i].fd);
            }
        }
    }
    close(fd_SerSocket);
}