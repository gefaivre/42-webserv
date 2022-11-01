NAME= webserv

SRC=	main_socket.cpp \
		Socket.cpp

INC=	Socket.hpp

CXX=c++

CXXFLAGS= -Wall -Wextra -Werror -g3 -std=c++98

OBJ=$(SRC:.c=.o)
	
all: $(NAME)

$(NAME): $(OBJ) $(INC)
	${CXX} $(CXXFLAGS) -o $(NAME) $(OBJ)
	@echo [$(NAME)]: Created !

clean:
	rm -rf *.o

fclean:	clean
	rm -rf $(NAME)
	@echo "[$(NAME)]: deleted"

re: fclean all

.PHONY: all, clean, fclean, re