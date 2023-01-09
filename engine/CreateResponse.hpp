#ifndef CREATERESPONSE_HPP
# define CREATERESPONSE_HPP

#include "utils.hpp"
#include "Server.hpp"
#include <string>

class Server;

char **ft_split(char *str);

class CreateResponse
{

	public:

		CreateResponse(Server *server,std::map<std::string, std::string> &requestMap, t_requestData requestData);

		~CreateResponse();


		std::string getResponse() const;
		std::string getHeaderResponse() const;
		std::string getBodyResponse() const;
		void displayHeaderResponse() const;
		void displayFullResponse() const;

	private:
		CreateResponse();

		std::string _path;
		bool 		_autoindex;

		Server *_server;

		t_requestData _requestData;

		
		std::map<std::string, std::string> _requestMap;


		
		t_headerData _headerData;

		std::vector<std::string> _responseHeader;
		std::map<std::string, std::string> _switchFilesExtension;

		//POST METHOD
		void collectData(int newsocket);

		void fillFilesExtension();
		void fillHeaderData();

		void createHeader();
		std::string _header;

		void createBody(bool firstTimeRead);
		void BodyIsNotIndex(bool firstTimeRead);
		void BodyIsIndex(bool firstTimeRead);
		std::string _body;
		FILE *_FILEtoRead;


};

// std::ostream &			operator<<( std::ostream & o, CreateResponse const & i );

#endif /* ************************************************** CREATERESPONSE_H */