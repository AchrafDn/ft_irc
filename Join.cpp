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
    if (command.size() > 3)
    {
        //need to send
        return;
    }
    std::string channel_name = command[1];
    std::string key = command[2];

    if (channel_name.c_str()[0] == '#')
    {
        for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
        {
            if ((*it)->GetName() == channel_name)
            {
                if ((*it)->GetLimit() != 0 && (*it)->GetClientCount() >= (*it)->GetLimit())
                {
                    std::string message = ":tinderhalal.com 471 " + client->get_nick() + " " + channel_name + " :Cannot join channel (+l)\r\n";
                    send(client->get_fd(), message.c_str(), message.size(), 0);
                    return;
                }
                // found the channel
                if (!((*it)->IsInviteOnly() && !(*it)->IsInvited(client)))
                {
                    if ((*it)->HasKey())
                    {
                        if ((*it)->GetKey() == key)
                        {
                            (*it)->AddUser(client);
                            client->set_last_channel(*it);
                            // std::cout << channels.back()->GetUsersList() << std::endl;
                            // Add the JOIN, 332, 333, 353, and 366 responses here
                            std::string topicmessage;
                            std::string topicSetByMessage;
                            if (client->get_last_channel()->GetTopic() == "")
                            {
                                topicmessage = ":tinderhalal.com 331 " + client->get_nick() + " " + channel_name + " :No topic is set\r\n";
                                topicSetByMessage = "";
                            }
                            else
                            {
                                topicmessage = ":tinderhalal.com 332 " + client->get_nick() + " " + channel_name + " :" + client->get_last_channel()->GetTopic() + "\r\n";
                                topicSetByMessage = ":tinderhalal.com 333 " + client->get_nick() + " " + channel_name + " " + client->get_last_channel()->GetTopicSetBy() + " " + client->get_last_channel()->GetTopicSetAt() + "\r\n";
                            }
                            std::string joinMessage = ":" + client->get_nick() + "!~" + client->get_user() + "@localhost JOIN " + channel_name + "\r\n";
                            std::string topicMessage = ":tinderhalal.com 332 " + client->get_nick() + " " + channel_name + " :" + client->get_last_channel()->GetTopic() + "\r\n";
                            std::string namesMessage = ":tinderhalal.com 353 " + client->get_nick() + " @ " + channel_name + " :" + client->get_last_channel()->GetUsersList() + "\r\n";
                            std::string endNamesMessage = ":tinderhalal.com 366 " + client->get_nick() + " " + channel_name + " :End of /NAMES list.\r\n";

                            std::string message = joinMessage + topicMessage + topicSetByMessage + namesMessage + endNamesMessage;
                            client->get_last_channel()->SendChannelMessage(message);
                        }
                        else
                        {
                            std::string message = ":tinderhalal.com 475 " + client->get_nick() + " " + channel_name + " :Cannot join channel (+k)\r\n";
                            send(client->get_fd(), message.c_str(), message.size(), 0);
                            return;
                        }
                    }
                    else
                    {
                        (*it)->AddUser(client);
                        client->set_last_channel(*it);
                        // std::cout << channels.back()->GetUsersList() << std::endl;
                        // Add the JOIN, 332, 333, 353, and 366 responses here
                        std::string topicmessage;
                        std::string topicSetByMessage;
                        if (client->get_last_channel()->GetTopic() == "")
                        {
                            topicmessage = ":tinderhalal.com 331 " + client->get_nick() + " " + channel_name + " :No topic is set\r\n";
                            topicSetByMessage = "";
                        }
                        else
                        {
                            topicmessage = ":tinderhalal.com 332 " + client->get_nick() + " " + channel_name + " :" + client->get_last_channel()->GetTopic() + "\r\n";
                            topicSetByMessage = ":tinderhalal.com 333 " + client->get_nick() + " " + channel_name + " " + client->get_last_channel()->GetTopicSetBy() + " " + client->get_last_channel()->GetTopicSetAt() + "\r\n";
                        }
                        std::string joinMessage = ":" + client->get_nick() + "!~" + client->get_user() + "@localhost JOIN " + channel_name + "\r\n";
                        std::string topicMessage = ":tinderhalal.com 332 " + client->get_nick() + " " + channel_name + " :" + client->get_last_channel()->GetTopic() + "\r\n";
                        std::string namesMessage = ":tinderhalal.com 353 " + client->get_nick() + " @ " + channel_name + " :" + client->get_last_channel()->GetUsersList() + "\r\n";
                        std::string endNamesMessage = ":tinderhalal.com 366 " + client->get_nick() + " " + channel_name + " :End of /NAMES list.\r\n";
                
                        std::string message = joinMessage + topicMessage + topicSetByMessage + namesMessage + endNamesMessage;
                        client->get_last_channel()->SendChannelMessage(message);
                    }
                    break;
                }
            }
        }
        if (client->get_last_channel() == NULL)
        {
            CreateChannel(command, client);
            // Add the JOIN, MODE, 353, and 366 responses here

            std::string joinMessage = ":" + client->get_nick() + "!~" + client->get_user() + "@0::1 JOIN " + channel_name + "\r\n";
            std::string modeMessage = ":tinderhalal.com MODE " + channel_name + " +nt\r\n";
            std::string namesMessage = ":tinderhalal.com 353 " + client->get_nick() + " = " + channel_name + " :@" + client->get_nick() + "\r\n";
            std::string endNamesMessage = ":tinderhalal.com 366 " + client->get_nick() + " " + channel_name + " :End of /NAMES list.\r\n";
            std::string message = joinMessage + modeMessage + namesMessage + endNamesMessage;
            client->set_last_channel(channels.back());
            channels.back()->SendChannelMessage(message);
            // std::cout << channels.back()->GetUsersList() << std::endl;
        }
    }
    else
    {
        std::string message = "Invalid channel name\r\n";
        send(client->get_fd(), message.c_str(), message.size(), 0);
    }
}