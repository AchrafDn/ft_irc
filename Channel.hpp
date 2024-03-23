#pragma once

#define MAX_NAME_LENGTH 50
#define MAX_USER_LIMIT 10

#include "Server.hpp"
#include "Client.hpp"

class Channel
{
private:
    size_t _limit;
    std::string _name;
    std::string _mode;
    std::string _topic;
    std::string _key;
    bool _hastopic;
    bool _haskey;
    long _creationtime;
    std::vector<Client *> _clients;
    std::vector<Client *> _invited;
    std::vector<Client *> _operators;
    Client               *_topic_setter;
    long                 _topic_time;

public:
    Channel();
    Channel(std::string name);
    Channel(std::string name, std::string mode = "", std::string topic, std::string key, Client *creator);
    virtual ~Channel();

    /*         GETTERS           */

    size_t GetLimit();
    std::string const &GetName() const;
    std::string const &GetMode() const;
    std::string const &GetTopic() const;
    std::string const &GetKey() const;
    size_t GetClientCount();
    bool const &HasTopic() const;
    bool const &HasKey() const;
    std::string const &GetCreationTime() const;

    /*         SETTERS           */

    void SetTopic(std::string &topic, Client *setter);
    void UnsetTopic(Client *setter);
    void SetKey(std::string &key);
    void UnsetKey();
    void SetMode(std::string &mode);
    void SetLimit(size_t limit);

    /*         CHECKERS          */

    int IsChannelOperator(Client *client);
    bool IsInvited(Client *client);
    bool IsInviteOnly();
    bool IsInChannel(Client *client);

    /*         METHODS           */

    void RemoveUser(Client *client);
    void AddUser(Client *client);
    void AddOperator(Client *client);
    void InviteUser(Client *client);
    std::string GetUsersList();

};