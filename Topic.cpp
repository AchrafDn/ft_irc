#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Topic(std::vector<std::string> command, Client *client)
{
    std::string response;
    if (command.size() == 1)
    {
        
        response = ":tinderhalal.com 461 :Not enough parameters\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        return;
    }
    if (command[1][0] != '#')
    {
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
        response = ":tinderhalal.com 403 :No such channel\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        
        return;
    }
    if (!channel->IsInChannel(client))
    {
        response = ":tinderhalal.com 442 :You're not on that channel\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        
        return;
    }
    if (command.size() == 2)
    {
        if (channel->GetTopic() == "")
        {
            
            response = ":tinderhalal.com 331 " + client->get_nick() + " " + channel->GetName() + " :No topic is set\r\n";
            send(client->get_fd(), response.c_str(), response.size(), 0);
            return;
        }
        std::string message = ":" + client->get_nick() + "!" + client->get_user() + "@tinderhalal.com TOPIC " + channel->GetName() + " :" + channel->GetTopic() + "\r\n";
        channel->SendChannelMessage(message);
        return;
    }
    if (command.size() == 3)
    {
        if (command[2][0] == ':')
        {
            command[2] = command[2].substr(1);
        }
        if (channel->IsChannelOperator(client))
        {
            std::string message = ":" + client->get_nick() + "!" + client->get_user() + "@tinderhalal.com TOPIC " + channel->GetName() + " :" + command[2] + "\r\n";
            channel->SetTopic(command[2], client);
            channel->SendChannelMessage(message);
        }
        else if (channel->IsTopicChangable())
        {
            std::string message = ":" + client->get_nick() + "!" + client->get_user() + "@tinderhalal.com TOPIC " + channel->GetName() + " :" + command[2] + "\r\n";
            channel->SetTopic(command[2], client);
            channel->SendChannelMessage(message);
        }
        else
        {
            response = ":tinderhalal.com 482 " + client->get_nick() + " " + channel->GetName() + " :You're not channel operator\r\n";      
            
        }
    }
}