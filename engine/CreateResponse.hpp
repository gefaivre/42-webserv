#ifndef CREATERESPONSE_HPP
# define CREATERESPONSE_HPP

# include "utils.hpp"
#include "general_includes.hpp"


class CreateResponse
{

	public:

		CreateResponse(std::string path, bool autoindex, t_requestData requestData);

		// CreateResponse( CreateResponse const & src );
		~CreateResponse();

		// CreateResponse &		operator=( CreateResponse const & rhs );

		std::string getResponse() const;

	private:
		CreateResponse();

		std::string _path;
		bool 		_autoindex;

		t_requestData _requestData;

		



		
		t_headerData _headerData;

		std::vector<std::string> _responseHeader;
		std::map<std::string, std::string> _switchFilesExtension;


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