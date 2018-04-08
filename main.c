#include "md.h"

int	main(int argc, char **argv)
{
	pid_t pID = fork();

	if (pID == 0)
	{
		if (getuid() == 0)
			return (ddaemon(argc, argv));
		else
		{
			printf("Not Root !\n");
			return (0);
		}
	}
	return (0);
}
