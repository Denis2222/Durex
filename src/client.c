#include "durex.h"

int			client_slot(void)
{
	for (int i = 0;i < MAX_CLIENT; i++) {
		if (get_durex()->clients[i].pid == -1) {
			return (i);
		}
	}
	return (-1);
}

void		new_client(t_client *client)
{
	char 	*buf;
	int 	run;
	int		ret;
	int		login = 0;
	run = 1;
	buf = (char *)malloc(100);

	dprintf(client->socket, "Enter password : (test)");
	while(run) {
		bzero(buf, 100);
		ret = read(client->socket, (void *)buf, 100);
		if (ret == 0) {
			run = 0;
			break;
		}
		if (ret > 0) {
			buf[ret - 1] = '\0';

			if (!login) {
				if (strcmp(buf, "test") == 0) { //TODO password
					dprintf(client->socket, "Welcome home : \n");
					login = 1;
				} else {
					run = 0;
				}

			} else if (login) { //Secure zone
 				if (strncmp(buf, "help", 4) == 0) {
					dprintf(client->socket, HELP);
				} else if (strncmp(buf, "shell", 5) == 0) {
					bash_prompt(client->socket);
				} else if (strncmp(buf, "quit", 4) == 0) {
					dprintf(client->socket, "Goodbye !\n");
					run = 0;
				} else {
					dprintf(client->socket, "%s command not found. Try : help \n", buf);
				}
			}
		}
	}
	close(client->socket);
	exit(0);
}
