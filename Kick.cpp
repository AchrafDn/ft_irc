#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Kick(std::vector<std::string> command, Client *client)
{
    std::string response;
    if (command.size() < 3)
    {
        // display error
        response = ":tinderhalal.com 461 :Not enough parameters\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        return;
    }
    if (command[1][0] != '#')
    {
        // display error
        response = ":tinderhalal.com 403 :No such channel\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        return;
    }
    Channel *channel = NULL;
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it)->GetName() == command[1])
        {
            channel = *it;
            break;
        }
    }
    if (channel == NULL)
    {
        // display error
        response = ":tinderhalal.com 403 :No such channel\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        return;
    }
    if (!channel->IsInChannel(client))
    {
        // display error
        response = ":tinderhalal.com 442 :You're not on that channel\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        return;
    }
    if (!channel->IsChannelOperator(client))
    {
        // display error
        response = ":tinderhalal.com 482 " + client->get_nick() + " " + channel->GetName() + " :You're not channel operator\r\n";
        std::cout << response;
        // send(client->get_fd(), response.c_str(), response.size(), 0);
        return;
    }
    Client *target = NULL;
    std::vector<Client*> clients = channel->GetClients();
    for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if ((*it)->get_nick() == command[2])
        {
            target = *it;
            break;
        }
    }
    if (target == NULL)
    {
        // display error
        response = ":tinderhalal.com 401 :No such nick/channel\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
    }
    std::string reason = (command.size() > 3) ? command[3] : client->get_nick();
    response = ":tinderhalal.com KICK " + command[1] + " " + command[2] + " :" + reason + "\r\n";
    channel->SendChannelMessage(response);
    channel->RemoveUser(target);
    target->set_last_channel(NULL);
    }