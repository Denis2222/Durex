#include "md.h"

int		build_status(void)
{
	return (1);
}

int		build_version(void)
{
	printf("Durex Ver 1.0.0, for debian-linux-gnu (x86_64)\n");
	return (0);
}

int		build_daemon(int argc, char **argv)
{
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "status") == 0)
			return (build_status());
		if (strcmp(argv[i], "version") == 0)
			return (build_version());
	}
	return (ddaemon(argc, argv));
}

int		has_root_rights(void)
{
	if (getuid() == 0)
		return (1);
	printf("Not Root !\n");
	return (0);
}

int	main(int argc, char **argv)
{
	daemon_install();
	if (has_root_rights() == 0)
		return (0);
	return (build_daemon(argc, argv));
}
