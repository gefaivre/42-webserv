NAME = webserv
SRCS = 	main.cpp \
		Parser.cpp Server.cpp \
		ft_split.cpp \
		Location.cpp \
		ft_strcmp_fowardslash.cpp \
		ft_remove_invalid_spaces.cpp \
		ft_remove_double_spaces.cpp \
		ft_wrap_in_spaces.cpp \
		ft_wrap_brackets_in_spaces.cpp \

OBJS_PATH = objects/
OBJS = ${SRCS:.cpp=.o}
CFLAGS = -Wall -Wextra -Werror -g3 -std=c++98
CC = c++
RM = rm -f
INC_PATH = ../includes/
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
