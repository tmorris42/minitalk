NAME = server
CLIENT = client
SERVER_SRCS = server.c
CLIENT_SRCS = client.c

SERVER_OBJS = ${SERVER_SRCS:.c=.o}
CLIENT_OBJS = ${CLIENT_SRCS:.c=.o}

GCC = gcc
#FLAGS = -Wall -Wextra -Werror
FLAGS = -Wall -Wextra

all: $(NAME) $(CLIENT)

$(SERVER_OBJS): %.o : %.c
	$(GCC) -c $(FLAGS) $(SERVER_SRCS)

$(CLIENT_OBJS): %.o : %.c
	$(GCC) -c $(FLAGS) $(CLIENT_SRCS)

$(NAME): $(SERVER_OBJS)
	$(GCC) $(FLAGS) $(SERVER_OBJS) -o $(NAME)

$(CLIENT): $(CLIENT_OBJS)
	$(GCC) $(CLIENT_SRCS) -o $(CLIENT)

clean:
	rm $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	rm $(NAME) $(CLIENT)

re: fclean all
