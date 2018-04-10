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
		d->clients[i].pid = -1;
		d->clients[i].socket = -1;
	}
	return (d);
}

t_durex	*get_durex(void)
{
	return (new_durex());
}

int durex_daemon(void)
{
	t_durex *durex = get_durex();

	//HANDLER OF SIGS
	signal(SIGCHLD, signal_handler);
	//START SOCKET SERVER
	if ((durex->socket = new_socket_server(PORT)) == -1)
		return (1);
	socket_server_handler(durex);
	return 0;
}
