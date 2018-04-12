.PHONY: all clean fclean re

NAME	= Durex
SRC		=	utils/ft_strnew.c													\
			utils/file_exists.c													\
			utils/is_directory.c												\
			utils/file_get_contents.c											\
			utils/file_put_contents.c											\
			utils/array_length.c												\
			utils/free_array.c													\
			utils/file_base_name.c												\
			utils/file_absolute_path.c											\
			utils/ft_split_string.c												\
			utils/ft_dstrjoin.c													\
			main.c																\
			durex.c																\
			daemon_install.c													\
			processes_exists.c													\
			shell.c																\
			socket.c															\
			client.c															\
			signals.c															\
			aes.c																\
			passwrd.c															\


dir_guard=@mkdir -p $(@D)

SRCDIR = ./src/
OBJDIR = ./obj

CC = gcc -I./include -lpthread -Wall -Wextra -Werror -std=gnu99 -lcrypto
SRCS=$(addprefix $(SRCDIR),$(SRC))
OBJS = $(addprefix $(OBJDIR)/, $(SRC:.c=.o))

.SILENT:

all: install_hexdump install_gcc install_openssl build_init $(NAME)

$(addprefix $(OBJDIR)/, %.o): $(addprefix $(SRCDIR), %.c)
	$(dir_guard)
	$(CC)  -o $@ -c $^
	printf "\033[38;5;092m%s\033[0m\033[K"	"COMPILE "
	printf "\033[38;5;092m%-35s\033[0m\033[K"	"[$^]"
	printf "\033[38;5;092m%s\033[0m\033[K\n"	" -> [$(@)]"

$(NAME): $(OBJS)
	$(CC) -o $@ $^  $(DL_FLAG)
	echo "Make [$(NAME)]"
	echo "\033[38;5;101mAuteurs :\033[K\033[0m"
	cat auteur

clean:
	rm -rf $(OBJS)
	echo -en "\r\033[38;5;101mCLEAN  "														\
		"[\033[0m$(NAME)\033[38;5;101m]\033[K\033[0m";

fclean: clean
	rm -rf $(OBJDIR)
	rm -rf $(NAME)
	echo -en "\r\033[38;5;124mFCLEAN "														\
		"[\033[0m$(NAME)\033[38;5;124m]\033[K\033[0m";

re: fclean all

FILE=include/daemon_init.h

install_hexdump:
	if test -f "/usr/bin/hexdump" ; then													\
		echo -en "\r\033[38;5;101mHEXDUMP"													\
			"[\033[0minstalled\033[38;5;101m]\033[K\033[0m";								\
	else																					\
		echo -en "\r\033[38;5;101mHEXDUMP"													\
			"[\033[0minstallation\033[38;5;101m]\033[K\033[0m";								\
		apt-get install gawk --yes;															\
		apt-get install bsdmainutils --yes;													\
	fi

install_gcc:
	if test -f "/usr/bin/gcc" ; then														\
		echo -en "\r\033[38;5;101mGCC"														\
			"    [\033[0minstalled\033[38;5;101m]\033[K\033[0m";							\
	else																					\
		echo -en "\r\033[38;5;101mGCC"														\
			"    [\033[0minstallation\033[38;5;101m]\033[K\033[0m";							\
		apt-get install build-essential --yes;												\
	fi

install_openssl:
	if test -f "/usr/bin/openssl" ; then													\
		echo -en "\r\033[38;5;101mOPENSSL"													\
			"[\033[0minstalled\033[38;5;101m]\033[K\033[0m";								\
	else																					\
		echo -en "\r\033[38;5;101mOPENSSL"													\
			"[\033[0minstallation\033[38;5;101m]\033[K\033[0m";								\
		apt-get install openssl --yes;														\
		apt-get install libssl-dev --yes;													\
	fi

build_init:
	printf "#ifndef DAEMON_INIT\n" > $(FILE)
	printf "# define DAEMON_INIT\n\n" >> $(FILE)
	printf "const char daemon_init[] = \"" >> $(FILE)
	hexdump -v -e '"\\""x" 1/1 "%02x" ""' durex.sh >> $(FILE)
	printf "\";\n" >> $(FILE)
	printf "const char daemon_init_systemd[] = \"" >> $(FILE)
	hexdump -v -e '"\\""x" 1/1 "%02x" ""' durex_systemd.sh >> $(FILE)
	printf "\";\n\n#endif" >> $(FILE)
	echo -en "\r\033[38;5;101mDAEMON "														\
		"[\033[0mbuilded\033[38;5;101m]\033[K\033[0m";										\

remove:
	rm -rf /usr/bin/Durex
	rm -rf /etc/init.d/Durex
	rm -rf /etc/systemd/system/Durex.service
	systemctl daemon-reload
	@pkill $(NAME)

ps:
	ps -aux | grep $(NAME)

test:
	rm -f ./serv
	gcc test.c -o serv
	./serv
