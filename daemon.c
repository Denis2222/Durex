#include "md.h"

#define OK 1
#define NOK 0

int	setup_env(t_env *e) {
	e->connected = 0;
	e->exit = 1;
	if (islock()) {
		return (NOK);
	}

	//Init client slots
	for (int i = 0; i < MAX_CLIENT; i++) {
		e->connect[i] = 0;
	}

	//Init Socket
	e->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (e->sock == -1) {
		return (NOK);
	}

	//Reuseaddr for socket quick clean !
	int optval = 1;
	setsockopt(e->sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));
	pthread_mutex_init(&e->mutex, NULL);

	e->sin.sin_addr.s_addr = htonl(INADDR_ANY);
	e->sin.sin_family = AF_INET;
	e->sin.sin_port = htons(PORT);

	if (bind(e->sock, (struct sockaddr *)&e->sin, sizeof(e->sin)) == -1) {
		return (NOK);
	}
	if (listen(e->sock, 5) == -1) {
		return (NOK);
	}
	return (OK);
}

int ddaemon(int argc, char **argv) {
	t_env		e;
	pthread_t	thread[3];
	setup_signal();

	(void)argc;
	(void)argv;
	if (setup_env(&e) == NOK)
		return (0);

	lock();
	while (e.exit) {
		t_sinsock *tss;
		tss = (t_sinsock*)malloc(sizeof(t_sinsock));
		tss->csinsize = sizeof(tss->csin);
		tss->csock = accept(e.sock, (struct sockaddr*)&tss->csin, &tss->csinsize);
		if (tss->csock < 0) {
			free(tss);
			continue;
		}

		tss->env = &e;

//		std::string newcon("Connection detected ! FROM : ");
//		newcon.append(inet_ntoa(tss->csin.sin_addr));

		int	full;

		full = 1;
		for(int i = 0; i < MAX_CLIENT; i++) {
			if (e.connect[i] == 0) {
				tss->thread = &thread[i];
				tss->nb = i;
				pthread_create(&thread[i], NULL, thread_client, (void *)tss);
				full = 0;
				i = MAX_CLIENT;
			}
		}
		if (full) {
			char str[]  = "Limit connection";
			write(tss->csock, str, strlen(str));
			close(tss->csock);
			free(tss);
		}
	}

	close(e.sock);
	unlock();
	return (0);
}
