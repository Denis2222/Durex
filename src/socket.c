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

int		new_socket_server(int port)
{
	int					addrlen;
	struct sockaddr_in	addr;
	int					sock;

	if (port < 0 || port > 32768)
		return (-1);
	addrlen = sizeof(struct sockaddr_in);
	memset(&addr, 0, addrlen);
	addr.sin_port        = htons(port);
	addr.sin_family      = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if((sock = socket( AF_INET, SOCK_STREAM, IPPROTO_IP)) == -1)
		return (-1);
	int	optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
		return (-1);
	if (bind(sock, (struct sockaddr*)&addr, addrlen) == -1)
		return (-1);
	if (listen(sock, SOCK_SEQPACKET) == -1)
		return (-1);
	return (sock);
}

void	socket_server_handler(t_durex *durex)
{
	int					addr_len;
	struct sockaddr_in	addr;
	int					sock;

	addr_len = sizeof(struct sockaddr_in);
	memset(&addr, 0, addr_len);
	while(-1 != (sock = accept(durex->socket, (struct sockaddr*)&addr, (socklen_t *)&addr_len)))
	{
		int slot_id = client_slot();
		if (slot_id >= 0)
		{
			const int sock_client = sock;
			const int client_id = slot_id;

			if(fork() == 0)
			{
				durex->clients[client_id].socket = sock_client;
				durex->clients[client_id].pid = getpid();
				new_client(&durex->clients[client_id]);
			}
		}
		else
		{
			dprintf(sock, "Already %d connected ! Go Away \n", MAX_CLIENT);
			write(sock, "", 0);
			close(sock);
		}
	}
}
