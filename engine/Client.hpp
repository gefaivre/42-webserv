#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <utils.hpp>

char **ft_split(const char *str);

class Client
{

	public:

		Client(int clientfd);
		// Client( Client const & src );
		~Client();

		void displayRequest();
		void readRequest();
		void sendResponse(std::string str);
		
		std::vector<std::string> getRequest() const;



	private:

		std::vector<std::string> _request;
		int _clientfd;


};

// std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */