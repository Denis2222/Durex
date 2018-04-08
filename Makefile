NAME	= Durex
SRC		=	main.c \
			lock.c \
			thread.c \
			daemon.c \
			signal.c

OBJ			= $(SRC:.c=.o)
CC			= gcc  -D_POSIX_C_SOURCE -std=c99 -lpthread -Wall -Werror -Wextra

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)
