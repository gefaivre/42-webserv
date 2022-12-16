#ifndef PARSINGREQUEST_HPP
# define PARSINGREQUEST_HPP

# include "utils.hpp"
#include "general_includes.hpp"
#include "Server.hpp"

class ParsingRequest
{

	public:

		ParsingRequest(std::vector<std::string> request, Server *server);
		// ParsingRequest( ParsingRequest const & src );
		~ParsingRequest();

		// ParsingRequest &		operator=( ParsingRequest const & rhs );

		t_requestData getData();

	private:
		ParsingRequest();
		std::string _path;
		bool 		_autoindex;


		std::vector<std::string> _request;

		t_requestData _requestData;

		Server *_server;

		void setFileToSend404();
		void setFileToSend403();


		void parsingRequest();
		int filepermission();

		int foundFileToSend();



};

// std::ostream &			operator<<( std::ostream & o, ParsingRequest const & i );

#endif /* ************************************************** PARSINGREQUEST_H */