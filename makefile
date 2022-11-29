NAME= webserv

SRC=	main.cpp \
		engine/utils.cpp \
		engine/Socket.cpp \
		engine/CreateResponse.cpp \
		engine/ParsingRequest.cpp 


INC =	engine/Socket.hpp \
		engine/CreateResponse.hpp \
		engine/ParsingRequest.hpp \
		engine/utils.hpp

CXX=c++

CXXFLAGS= -Wall -Wextra -Werror -g3 -std=c++98

OBJ=$(SRC:.c=.o)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INC)

all: $(NAME)

$(NAME): $(OBJ) 
	${CXX} $(CXXFLAGS) $(OBJ) -o $(NAME)
	@echo [$(NAME)]: Created !

clean:
	rm -rf *.o

fclean:	clean
	rm -rf $(NAME)
	@echo "[$(NAME)]: deleted"

re: fclean all

.PHONY: all, clean, fclean, re