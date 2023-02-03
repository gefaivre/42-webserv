NAME= webserv

SRCS=	engine/main.cpp \
		engine/utils.cpp \
		engine/Client.cpp \
		engine/CGI.cpp \
		engine/CreateResponse.cpp \
		engine/ParsingRequest.cpp\
		engine/saveFile.cpp\
		engine/epolling.cpp\
		engine/ft_split_header.cpp\
		parsing_conf/Parser.cpp \
		parsing_conf/Server.cpp \
		parsing_conf/Config.cpp\
		parsing_conf/ft_split.cpp \
		parsing_conf/Location.cpp \
		parsing_conf/ft_strcmp_fowardslash.cpp \
		parsing_conf/ft_remove_invalid_spaces.cpp \
		parsing_conf/ft_remove_double_spaces.cpp \
		parsing_conf/ft_wrap_in_spaces.cpp \
		parsing_conf/ft_wrap_brackets_in_spaces.cpp \
		parsing_conf/ft_strcmp.cpp \
		parsing_conf/ft_strncmp.cpp \
		parsing_conf/ft_starts_with.cpp

OBJS_PATH = objects/
OBJS = ${SRCS:.cpp=.o}
CFLAGS = -Wall -Wextra -Werror -g3 -std=c++98
CC = c++
RM = rm -f
INC_PATH = ./includes/
INCLUDES = $(addprefix -I , $(INC_PATH))

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

all: ${NAME}

$(NAME): $(OBJS)
		$(CC) ${CFLAGS} $(OBJS) -o $(NAME)


clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}
	${RM} -r ${OBJS_PATH}

re: fclean all

.PHONY: all clean fclean re
