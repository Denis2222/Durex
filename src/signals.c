#include "durex.h"

#include <sys/wait.h>

void	signal_child_handler(int sig)
{
	(void)sig;
	wait(NULL);
}

void	signal_all_handler(int sig)
{
	(void)sig;
	daemon_remove_lock();
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (get_durex()->clients[i].used == true) {
			get_durex()->clients[i].used = false;
			write(get_durex()->clients[i].socket, "", 0);
			close(get_durex()->clients[i].socket);
			get_durex()->clients[i].socket = -1;
		}
	}
	wait(NULL);
	destruct_durex(get_durex());
	exit(0);
}
