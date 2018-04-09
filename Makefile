NAME	= Durex
SRC		=	main.c \
			lock.c \
			thread.c \
			daemon.c \
			signal.c


SRCDIR = ./src/
OBJDIR = ./obj

CC = gcc -I./include -D_POSIX_C_SOURCE -std=c99 -lpthread -Wall -Werror -Wextra 
SRCS=$(addprefix $(SRCDIR),$(SRC))
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: $(NAME)

$(addprefix $(OBJDIR)/, %.o): $(addprefix $(SRCDIR)/, %.c)
	$(dir_guard)
	@$(CC)  -o $@ -c $^

$(NAME): $(OBJS)
	@$(CC) -o $@ $^  $(DL_FLAG)
	@echo "[$@] Complete"


all: $(NAME)

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(OBJDIR)
	rm -rf $(NAME)

re: fclean $(NAME)
