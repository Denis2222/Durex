NAME	= Durex
SRC		=	main.c \
			durex.c \
			daemon_install.c \


dir_guard=@mkdir -p $(@D)

SRCDIR = ./src/
OBJDIR = ./obj

CC = gcc -I./include -lpthread -Wall -Wextra -Werror
SRCS=$(addprefix $(SRCDIR),$(SRC))
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

all: build_init $(NAME)

$(addprefix $(OBJDIR)/, %.o): $(addprefix $(SRCDIR)/, %.c)
	$(dir_guard)
	@$(CC)  -o $@ -c $^

$(NAME): $(OBJS)
	@$(CC) -o $@ $^  $(DL_FLAG)
	@echo "[$@] Complete"

clean:
	rm -rf $(OBJS)

fclean: clean
	rm -rf $(OBJDIR)
	rm -rf $(NAME)

re: fclean all

FILE=include/daemon_init.h

build_init:
	@printf "#ifndef DAEMON_INIT\n" > $(FILE)
	@printf "# define DAEMON_INIT\n\n" >> $(FILE)
	@printf "const char daemon_init[] = \"" >> $(FILE)
	@hexdump -v -e '"\\""x" 1/1 "%02x" ""' durex.sh >> $(FILE)
	@printf "\";\n\n#endif" >> $(FILE)


test:
	rm -f ./serv
	gcc test.c -o serv
	./serv
