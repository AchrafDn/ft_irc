#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Part(std::vector<std::string> command, Client *client)
{
    if (command.size() != 2)
    {
        // display error
        return;
    }
    if (command[1][0] != '#')
    {
        // display error
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
        return;
    }
    if (!channel->IsInChannel(client))
    {
        // display 442 error
        std::string message = ":tinderhalal.com 442 " + client->get_nick() + " " + channel->GetName() + " :You're not on that channel\r\n";
        send(client->get_fd(), message.c_str(), message.size(), 0);
        return;
    }
    if (channel->IsChannelOperator(client))
    {
        std::string channel_name = channel->GetName();
        if (channel->GetClientCount() == 1)
        {
            RemoveChannel(channel);
            return;
        }
        std::string message = ":" + client->get_nick() + "!" + client->get_user() + "@tinderhalal.com PART " + channel_name + "\r\n";
        channel->RemoveUser(client);
        channel->RemoveOperator(client->get_nick());
        client->set_last_channel(NULL);
        channel->SendChannelMessage(message);
    }
    else
    {
        std::string message = "PART " + channel->GetName() + " " + client->get_nick() + "\r\n";
        channel->RemoveUser(client);
        channel->SendChannelMessage(message);
        client->set_last_channel(NULL);
    }
}