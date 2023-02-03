#ifndef CREATERESPONSE_HPP
# define CREATERESPONSE_HPP

#include "utils.hpp"
#include "Server.hpp"
#include <string>
#include "define.hpp"

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
		int create();

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
	
		void errorStatus();
		int checkErrorPage(std::string errorCode, std::string path);


		void createHeader();
		std::string _header;

		int createBody();
		int BodyIsCgi();
		int BodyIsNotIndex();
		int BodyIsIndex();
		std::string _body;
		FILE *_FILEtoRead;

		// EPOLLING
		bool _createBody;
		bool _fillHeaderData;
		bool _createHeader;
		bool _firstTimeBody;
		bool _bodyIsRead;
		std::ifstream _myfile;



};

// std::ostream &			operator<<( std::ostream & o, CreateResponse const & i );

#endif /* ************************************************** CREATERESPONSE_H */