#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Mode(std::vector<std::string> command, Client *client)
{
    std::string response;
    Channel *channel = NULL;
    if (command.size() <= 2)
    {
        response = ":tinderhalal.com 461 :Not enough parameters\r\n";
        return;
    }
    if (command.size() > 4)
        return;
    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it)->GetName() == command[1])
        {
            channel = *it;
            break;
        }
    }
    if(channel == NULL)
    {
        response = ":tinderhalal.com 403 :No such channel\r\n";
        return;
    }
    if (!channel->IsInChannel(client))
    {
        response = ":tinderhalal.com 442 " + client->get_nick() + " " + channel->GetName() + " :You're not on that channel\r\n";
        return;
    }
    if(channel->GetClientByNick(client->get_nick()) == "")
    {
        //401 ERR_NOSUCHNICK
        response = ":tinderhalal.com 401 " + client->get_nick() + " " + command[1] + " :No such nick/channel\r\n";
        return;
    }
    if (!(channel->IsChannelOperator(client)))
    {
        response = ":tinderhalal.com 482 " + client->get_nick() + " " + channel->GetName() + " :You're not channel operator\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        return ;
    }
    if (command[1][0] != '#')
    {
        response = ":tinderhalal.com 403 :No such channel\r\n";
        send(client->get_fd(), response.c_str(), response.size(), 0);
        return;
    }
    std::string mode;
    std::string argument;
    if (channel->IsInChannel(client))
    {
        if (command.size() == 3)
        {
            mode = command[2];
        }
        else if (command.size() == 4)
        {
            mode = command[3];
            argument = command[2];
        }
    }
    else
    {
        // display error
        return;
    }
    if (command.size() == 4 && (command[3][0] == '+' || command[3][0] == '-'))
    {
        argument = command[2];
        mode = command[3];
    }
    else if (command[2][0] == '+' || command[2][0] == '-')
    {
        argument = command[3];
        mode = command[2];
    }
    else
    {
        // display error
        return;
    }
    bool set = false;
    bool chanop = false;
    for (std::size_t i = 0; i < mode.size(); ++i)
    {
        char c = mode[i];
        if (c == '+')
            set = true;
        else if (c == '-')
            set = false;
        else if (c == 'i')
        {
            channel->SetInviteOnly(set);
            if (set)
                response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :+i\r\n";
            else
                response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :-i\r\n";
            channel->SendChannelMessage(response);
        }
        else if (c == 't')
        {
            channel->SetOpsChangeTopic(set);
            if (set)
                response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :+t\r\n";
            else
                response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :-t\r\n";
            channel->SendChannelMessage(response);
        }
        else if (c == 'k' && !chanop)
        {
            chanop = true;
            if (set)
            {
                channel->SetKey(argument);
                response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :+k " + argument + "\r\n";
            }
            else
            {
                channel->UnsetKey();
                response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :-k\r\n";
            }
            channel->SendChannelMessage(response);
        }
        else if (c == 'l' && !chanop)
        {
            chanop = true;
            if (set)
            {
                int limit;
                //in case its not an integer
                try
                {
                    int pos = 0;
                    while (pos < (int)argument.size() && isdigit(argument[pos]))
                    {
                        pos++;
                    }
                    limit = std::stoi(argument.substr(0, pos));
                    channel->SetLimit(limit);
                    response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :+l " + std::to_string(limit) + "\r\n";
                }
                catch (const std::exception &e)
                {
                    continue;
                }
            }
            else
            {
                channel->SetLimit(MAX_USER_LIMIT);
                response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :-l\r\n";
            }
            channel->SendChannelMessage(response);
        }
        else if (c == 'o' && !chanop)
        {
            chanop = true;
            if (set)
            {
                if(channel->AddOperator(argument))
                    response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :+o " + argument + "\r\n";
                else
                    response = ":tinderhalal.com 441 " + client->get_nick() + " " + argument + " :They aren't on that channel\r\n";
                channel->SendChannelMessage(response);
            }
            else
            {
                if(channel->RemoveOperator(argument))
                    response = ":" + client->get_nick() + " MODE " + channel->GetName() + " :-o " + argument + "\r\n";
                else
                    response = ":tinderhalal.com 441 " + client->get_nick() + " " + argument + " :They aren't on that channel\r\n";
            }
        }
    }

}