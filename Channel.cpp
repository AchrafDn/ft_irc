#include "Channel.hpp"

Channel::Channel()
{
    _limit = MAX_USER_LIMIT;
    _name = "";
    _mode = "";
    _topic = "";
    _key = "";
    _hastopic = false;
    _haskey = false;
    time_t creation = time(NULL);
    _creationtime = (intmax_t)creation;
}

Channel::Channel(std::string name)
{
    _limit = MAX_USER_LIMIT;
    _name = name;
    _mode = "";
    _topic = "";
    _key = "";
    _hastopic = false;
    _haskey = false;
    time_t creation = time(NULL);
    _creationtime = (intmax_t)creation;
}

Channel::Channel(std::string name, std::string mode, std::string topic, std::string key, Client *creator)
{
    _limit = MAX_USER_LIMIT;
    _name = name;
    _mode = mode;
    _topic = topic;
    _key = key;
    _hastopic = true;
    _haskey = true;
    time_t creation = time(NULL);
    _creationtime = (intmax_t)creation;

    if (key != "")
    {
        _haskey = true;
        _mode += "k";
    }

    if (topic != "")
    {
        _hastopic = true;
    }

    _operators.push_back(creator);
    AddUser(creator);
}

Channel::~Channel()
{
}

    /*         GETTERS           */

size_t Channel::GetLimit()
{
    return _limit;
}

std::string const &Channel::GetName() const
{
    return _name;
}

std::string const &Channel::GetMode() const
{
    return _mode;
}

std::string const &Channel::GetTopic() const
{
    return _topic;
}

std::string const &Channel::GetKey() const
{
    return _key;
}

size_t Channel::GetClientCount()
{
    return _clients.size();
}

bool const &Channel::HasTopic() const
{
    return _hastopic;
}

bool const &Channel::HasKey() const
{
    return _haskey;
}

std::string const &Channel::GetCreationTime() const
{
    std::ostringstream os;
    os << std::fixed << _creationtime;
    std::string str = os.str();
    return str;
}

    /*         SETTERS           */

void Channel::SetTopic(std::string &topic, Client *setter)
{
    _topic = topic;
    _topic_setter = setter;
    time_t rn = time(0);
    _topic_time = (long)rn;
}

void Channel::UnsetTopic(Client *setter)
{
    _topic = "";
    _topic_setter = setter;
    time_t rn = time(0);
    _topic_time = (long)rn;
}


void Channel::SetKey(std::string &key)
{
    _key = key;
    _haskey = true;
}

void Channel::UnsetKey()
{
    _key = "";
    _haskey = false;
}

void Channel::SetMode(std::string &mode)
{
    _mode = mode;
}

void Channel::SetLimit(size_t limit)
{
    if (limit < 0 && limit > MAX_USER_LIMIT)
        _limit = MAX_USER_LIMIT;
    else
        _limit = limit;
}

    /*         CHECKERS          */

int Channel::IsChannelOperator(Client *client)
{
    for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); it++)
    {
        if (*it == client)
            return 1;
    }
    return 0;
}

bool Channel::IsInvited(Client *clien)
{
    for (std::vector<Client *>::iterator it = _invited.begin(); it != _invited.end(); it++)
    {
        if (*it == clien)
            return true;
    }
    return false;
}

bool Channel::IsInviteOnly()
{
    if (_mode.find("i") != std::string::npos)
        return true;
    return false;
}

bool Channel::IsInChannel(Client *client)
{
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (*it == client)
            return true;
    }
    return false;
}

    /*         METHODS           */

void Channel::AddUser(Client *client)
{
    _clients.push_back(client);
}

void Channel::RemoveUser(Client *client)
{
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (*it == client)
        {
            if (IsChannelOperator(client))
            {
                // looping to remove operator
                for (std::vector<Client *>::iterator it = _operators.begin(); it != _operators.end(); it++)
                {
                    if (*it == client)
                    {
                        _operators.erase(it);
                        break;
                    }
                }
            }
            _clients.erase(it);
            break;
        }
    }
}

void Channel::AddOperator(Client *client)
{
    _operators.push_back(client);
}

void Channel::InviteUser(Client *client)
{
    _invited.push_back(client);
}


std::string Channel::GetUsersList()
{
    std::string users;
    for (std::vector<Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (IsChannelOperator(*it))
            users += "@";
        else
            users += (*it)->get_nick() + " ";
    }
    return users;
}

