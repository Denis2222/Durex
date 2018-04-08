#include "md.hpp"

#define OK 1
#define NOK 0

int	setup_env(t_env *e) {
	e->connected = 0;
	e->exit = 1;
	e->report.Logstd("Started", INFO);
	if (islock()) {
		e->report.Logstd("Already Start .lock exist", ERROR);
		return (NOK);
	}

	//Init client slots
	for (int i = 0; i < MAX_CLIENT; i++) {
		e->connect[i] = 0;
	}

	//Init Socket
	e->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (e->sock == -1) {
		e->report.Logstd("Socket() Error", ERROR);
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
		e->report.Logstd("Bind() Error Port already in use !", ERROR);
		return (NOK);
	}
	if (listen(e->sock, 5) == -1) {
		e->report.Logstd("Listen() Error", ERROR);
		return (NOK);
	}
	return (OK);
}

int daemon(int argc, char **argv) {
	t_env		e;
	pthread_t	thread[3];
	ge = &e;
	setup_signal();
	if (setup_env(&e) == NOK)
		return (0);

	lock();
	e.report.Logstd("Daemon Ready !", INFO);
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

		std::string newcon("Connection detected ! FROM : ");
		newcon.append(inet_ntoa(tss->csin.sin_addr));
		e.report.Logstd(newcon , INFO);

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
			e.report.Logstd("Max connected ! reject ", INFO);
			char str[]  = "Limit connection";
			write(tss->csock, str, strlen(str));
			close(tss->csock);
			free(tss);
		}
	}

	close(e.sock);
	e.report.Logstd(" Exit Program ", INFO);
	unlock();
	return (0);
}
