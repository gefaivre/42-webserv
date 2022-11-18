NAME= webserv

SRC=	main_socket.cpp \
		Socket.cpp \
		parsing/ft_split.cpp \
		parsing/Parser.cpp \
		parsing/Server.cpp


INC_PATH =	includes/
INC = $(addprefix -I , $(INC_PATH))
CXX=c++

CXXFLAGS= -Wall -Wextra -Werror -g3 -std=c++98

OBJ=$(SRC:.c=.o)
	
all: $(NAME)

$(NAME): $(OBJ) 
	${CXX} $(CXXFLAGS) -o $(NAME) $(OBJ) $(INC)
	@echo [$(NAME)]: Created !

clean:
	rm -rf *.o

fclean:	clean
	rm -rf $(NAME)
	@echo "[$(NAME)]: deleted"

re: fclean all

.PHONY: all, clean, fclean, re