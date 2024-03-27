#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

void Server::Mode(std::vector<std::string> command, Client *client)
{
    if(client->get_last_channel() != NULL)
    {
        if (command.size() != 3 && command.size() != 4)
            return;
        if (!(client->get_last_channel()->IsChannelOperator(client)))
        {
            // display error
            return ;
        }

        std::string mode;
        std::string argument;
        if (command[3][0] == '+' || command[3][0] == '-')
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
                client->get_last_channel()->SetInviteOnly(set);
            }
            else if (c == 't')
            {
                client->get_last_channel()->SetOpsChangeTopic(set);
            }
            else if (c == 'k' && !chanop)
            {
                chanop = true;
                if (set)
                {
                    std::cout << "k set called" << std::endl;
                    client->get_last_channel()->SetKey(argument);
                }
                else
                {
                    std::cout << "k unset called" << std::endl;
                    client->get_last_channel()->UnsetKey();
                }
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
                        std::cout << "l set called" << std::endl;
                        client->get_last_channel()->SetLimit(limit);
                    }
                    catch (const std::exception &e)
                    {
                        continue;
                    }
                }
                else
                {
                    std::cout << "l unset called" << std::endl;
                    client->get_last_channel()->SetLimit(MAX_USER_LIMIT);
                }
            }
            else if (c == 'o' && !chanop)
            {
                chanop = true;
                if (set)
                {
                    if(client->get_last_channel()->AddOperator(argument))
                        std::cout << "o set called" << std::endl;
                }
                else
                {
                    if(client->get_last_channel()->RemoveOperator(argument))
                        std::cout << "o unset called" << std::endl;
                }
            }
        }
    }

}