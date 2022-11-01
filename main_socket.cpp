#include "Socket.hpp"

int main(int ac, char **av)
{
    if (ac != 2)
    {
        std::cout << "Usage: ./webserv [port]" << std::endl;
        return (0);
    }
    Socket socket(atoi(av[1]));

    socket.acceptAndRead();

}