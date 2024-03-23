#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::AddChannel(Channel *channel)
{
    channels.push_back(channel);
}

void Server::CreateChannel(std::vector<std::string> command, Client *creator)
{
    std::string name = command[1];
    std::string key = command[2];

    Channel *channel = new Channel(name, "nt", "", key, creator);
    AddChannel(channel);
}

void Server::RemoveChannel(Channel *channel)
{
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if (*it == channel)
        {
            channels.erase(it);
            delete channel;
            break;
        }
    }
}

void Server::Join(std::vector<std::string> command, Client *client)
{
    std::string channel_name = command[1];
    std::string key = command[2];

    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        if ((*it)->GetName() == channel_name)
        {
            if (key != "")
            {
                if ((*it)->GetKey() == key)
                {
                    (*it)->AddUser(client);
                    client->set_last_channel(*it);
                }
                else
                {
                    std::string message = "Wrong key\r\n";
                    send(client->get_fd(), message.c_str(), message.size(), 0);
                }
            }
            else
            {
                (*it)->AddUser(client);
                client->set_last_channel(*it);
            }
            break;
        }
    }
}