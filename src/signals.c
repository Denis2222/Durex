#include "durex.h"

#include <sys/wait.h>

void	signal_handler(int sig)
{
	(void)sig;
	wait(NULL);
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (get_durex()->clients[i].used == true) {
			get_durex()->clients[i].used = false;
			write(get_durex()->clients[i].socket, "", 0);
			close(get_durex()->clients[i].socket);
			get_durex()->clients[i].socket = -1;
			destruct_durex(get_durex());
			exit(0);
		}
	}
}
