#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <cerrno>

int main()
{
    int sockfd;

    struct sockaddr_in addr;
    memset((char *)&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = INADDR_ANY; // indique que toutes les sources seront acceptées
    addr.sin_port = htons(2299); // toujours penser à traduire le port en réseau
    addr.sin_family = AF_INET; // notre socket est TCP

    if ((sockfd = socket(AF_INET,SOCK_STREAM, 0)) == -1)
    {
        std::cout << "Socket error" << std::endl;
        std::cout << errno << std::endl;
        return(0);
    }


    if( (bind(sockfd, (struct sockaddr*)&addr, sizeof(addr))) == -1)
    {
        std::cout << "Bind error" << std::endl;
        std::cout << errno << std::endl;
        return (0);
    }

    if (listen(sockfd, SOMAXCONN) == -1)
    {
        std::cout << "Listen error" << std::endl;
        std::cout << errno << std::endl;
        return (0); 
    }

        int new_socket;
        socklen_t addrlen = sizeof(addr);

        new_socket = accept(sockfd, (struct sockaddr *)&addr, &addrlen);

        if (new_socket == -1)
        {
            std::cout << "Accept error" << std::endl;
            std::cout << errno << std::endl;
            return (0);
        }
        
        char buffer[10000] = {0};
        if ( read( new_socket , buffer, 10000) == 0)
        {
            std::cout << "No Bytes are there to read" << std::endl;
        }
        printf("%s",buffer );
        write(new_socket , "hello\n" , strlen("hello\n"));
        close(new_socket);

    return (0);
}