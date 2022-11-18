NAME = webserv
SRCS = main.cpp Parser.cpp Server.cpp ft_split.cpp

OBJS_PATH = objects/
OBJS = ${SRCS:.cpp=.o}
CFLAGS = -Wall -Wextra -Werror -std=c++98
CC = c++
RM = rm -f

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

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
