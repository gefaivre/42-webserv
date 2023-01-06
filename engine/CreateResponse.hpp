#ifndef CREATERESPONSE_HPP
# define CREATERESPONSE_HPP

#include "utils.hpp"
#include "Server.hpp"

class Server;

char **ft_split(char *str);

class CreateResponse
{

	public:

		CreateResponse(Server *server, t_requestData requestData);

		~CreateResponse();


		std::string getResponse() const;
		void displayHeaderResponse() const;
		void displayFullResponse() const;

	private:
		CreateResponse();

		std::string _path;
		bool 		_autoindex;

		Server *_server;

		t_requestData _requestData;

		



		
		t_headerData _headerData;

		std::vector<std::string> _responseHeader;
		std::map<std::string, std::string> _switchFilesExtension;

		//POST METHOD
		void collectData(int newsocket);

		void createAndSendResponse();
		void fillFilesExtension();
		void fillHeaderData();

		void createHeader();
		std::string _header;

		void createBody();
		void BodyIsNotIndex();
		void BodyIsIndex();
		std::string _body;


		void joinHeaderBody();

		std::string _response;


};

// std::ostream &			operator<<( std::ostream & o, CreateResponse const & i );

#endif /* ************************************************** CREATERESPONSE_H */