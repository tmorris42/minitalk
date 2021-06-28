NAME = server
CLIENT = client
SERVER_SRCS = server.c messages.c
CLIENT_SRCS = client.c
HEADERS = minitalk.h

SERVER_OBJS = ${SERVER_SRCS:.c=.o}
CLIENT_OBJS = ${CLIENT_SRCS:.c=.o}

GCC = gcc
FLAGS = -Wall -Wextra -Werror

all: $(NAME) $(CLIENT)

$(SERVER_OBJS): %.o : %.c $(HEADERS)
	$(GCC) -c $(FLAGS) $(SERVER_SRCS)

$(CLIENT_OBJS): %.o : %.c $(HEADERS)
	$(GCC) -c $(FLAGS) $(CLIENT_SRCS)

$(NAME): $(SERVER_OBJS)
	$(GCC) $(FLAGS) $(SERVER_OBJS) -o $(NAME)

$(CLIENT): $(CLIENT_OBJS)
	$(GCC) $(FLAGS) $(CLIENT_SRCS) -o $(CLIENT)

clean:
	rm $(SERVER_OBJS) $(CLIENT_OBJS)

fclean: clean
	rm $(NAME) $(CLIENT)

re: fclean all

test: $(NAME) $(CLIENT) test.py
	@python3.7 test.py

.PHONY: all clean fclean re test
