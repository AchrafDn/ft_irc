#include "Server.hpp"
#include <ctime>

int main(int ac, char **av){
    if(ac != 3)
    {
        std::cout << "Syntax of input is invalid" << std::endl;
        return 1;
    }

    Server ser(std::atoi(av[1]), av[2]);

    std::time_t serv_timeCreation = std::time(NULL);
    ser.set_time(std::ctime(&serv_timeCreation));


    try{
        ser.execution();
    }
    catch(std::string e)
    {
        std::cout << e << std::endl;
        return 1;
    }
    return 0;
}