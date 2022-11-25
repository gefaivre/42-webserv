#ifndef PARSINGREQUEST_HPP
# define PARSINGREQUEST_HPP

# include <iostream>
# include <string>

# include "utils.hpp"
# include <vector>
# include <cstdio>
# include <cerrno>

class ParsingRequest
{

	public:

		ParsingRequest(std::string path, bool autoindex, std::vector<std::string> request);
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


		void parsingRequest();

		int foundFileToSend();



};

// std::ostream &			operator<<( std::ostream & o, ParsingRequest const & i );

#endif /* ************************************************** PARSINGREQUEST_H */