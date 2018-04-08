#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>

#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <signal.h> // sigaction(), sigsuspend(), sig*()
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>

#define PORT       4242
#define MAX_CLIENT 3
#define LOCK_PATH  "/var/lock/matt_daemon.lock"
#define LOG_PATH   "/var/log/matt_daemon/matt_daemon.log"
#define LOG_FOLDER "/var/log/matt_daemon/"

//#define LOCK_PATH  "./matt_daemon.lock"
//#define LOG_PATH   "./matt_daemon.log"

enum					e_logtype
{
	INFO,
	LOG,
	ERROR
};

#include "Tintin_reporter.hpp"

typedef struct			s_sinsock
{
	int					csock;
	struct sockaddr_in	csin;
	socklen_t			csinsize;
	int					nb;
	pthread_mutex_t		*mutex;
	pthread_t			*thread;
	struct s_env		*env;
}						t_sinsock;

typedef struct			s_env
{
	int					sock; //Server
	int					exit;
	struct sockaddr_in	sin;

	Tintin_reporter		report;

	//Unsafe
	int					connected;
	pthread_mutex_t		mutex;

	int					connect[MAX_CLIENT];
	t_sinsock			client[MAX_CLIENT];
}						t_env;


int		islock();
void	lock();
void	unlock();

void	*thread_client(void *sin_sock);
int		daemon(int argc, char **argv);
void	sighandler(int signo);
void	setup_signal();

static t_env *ge;

