#include "durex.h"

#include <sys/wait.h>

void	signal_handler(int sig)
{
	pid_t pid;

	(void)sig;
	pid = wait(NULL);
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (get_durex()->clients[i].pid == pid) {
			get_durex()->clients[i].pid = -1;
			write(get_durex()->clients[i].socket, "", 0);
			close(get_durex()->clients[i].socket);
			get_durex()->clients[i].socket = -1;
		}
	}
}
