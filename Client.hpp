#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "Server.hpp"
#include "Client.hpp"
#include "Channel.hpp"

class Channel;
class Client;
class Server;

class Client{
    private:
        int fd;
        std::string name;
        std::string nick;
        std::string user;
        std::string real_name;
        int registered;
        Channel *last_channel;

    public:
        Client() : fd(0), name(""), nick(""), registered(0), last_channel(NULL){}
        Client(int _fd, int _registered) : fd(_fd), name(""), nick(""), registered(_registered), last_channel(NULL){}
        Client(int _fd, std::string _name, std::string _nick) : fd(_fd), name(_name), nick(_nick), last_channel(NULL){};
        
        void    set_fd(int _fd) {fd = _fd;}
        void    set_nick(std::string _nick) {nick = _nick;}
        void    set_name(std::string _name) {name = _name;}
        void    set_real_name(std::string _real_name) {real_name = _real_name;}
        void    set_registered(int num) {registered = num;}
        void    set_user(std::string _user) {user = _user;}
        void    set_last_channel(Channel *channel) {last_channel = channel;}

        int    get_fd() {return fd;}
        int    get_registered() {return registered;}
        std::string    get_nick() {return nick;}
        std::string    get_name() {return name;}
        std::string    get_real_name() {return real_name;}
        std::string    get_user() {return user;}
        Channel    *get_last_channel() {return last_channel;}

};

#endif