#include "durex.h"

#include <sys/wait.h>

void	signal_handler(int sig)
{
	t_durex *durex;

	//wait childs process
	wait(NULL);
	(void)sig;
	durex = get_durex();
	for (int i = 0; i < MAX_CLIENT; i++)
	{
		if (durex->clients[i].used == true)
		{
			durex->clients[i].used = false;
			write(durex->clients[i].socket, "", 0);
			close(durex->clients[i].socket);
			durex->clients[i].socket = -1;
		}
	}
	destruct_durex(durex);
	exit(0);
}
