NAME	= Matt_daemon
SRC		=	main.cpp \
			lock.cpp \
			thread.cpp \
			daemon.cpp \
			signal.cpp \
			Tintin_reporter.cpp

OBJ			= $(SRC:.cpp=.o)
CC			= g++ -lpthread -std=c++11 -Wall -Werror -Wextra

$(NAME): $(OBJ)
	$(CC) $(OBJ) -o $(NAME)

all: $(NAME)

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(NAME)

re: fclean $(NAME)
