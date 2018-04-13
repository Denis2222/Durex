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

t_durex	*new_durex(void)
{
	static t_durex *d = NULL;

	if (d != NULL)
		return (d);
	if (!(d = (struct s_durex*)malloc(sizeof(struct s_durex))))
		return (NULL);
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		d->clients[i].used = false;
		d->clients[i].socket = -1;
		d->clients[i].shell_port = -1;
		d->clients[i].shell_pid = -1;
	}
	for (int i = 0; i < MAX_SHELLS; i++)
	{
		d->shell_ports[i] = SHELL_START_PORT + i;
		d->shell_ports_used[i] = false;
	}
	d->program_path = NULL;
	return (d);
}

t_durex	*get_durex(void)
{
	return (new_durex());
}

void	destruct_durex(t_durex *durex)
{
	if (durex->program_path != NULL)
		free(durex->program_path);
	free(durex);
}

int durex_daemon(char *program_path)
{
	t_durex *durex = get_durex();

	if (durex == NULL)
		return (1);
	daemon_create_lock();
	durex->program_path = strdup(program_path);
	//HANDLER OF SIGS
	signal(SIGCHLD, signal_child_handler);
	for (int i = 1; i <= 15; i++)
		signal(i, signal_all_handler);
	//START SOCKET SERVER
	if ((durex->socket = new_socket_server(PORT)) == -1)
	{
		destruct_durex(durex);
		return (1);
	}
	socket_server_handler(durex);
	destruct_durex(durex);
	return 0;
}
