#include "md.h"
#include "daemon_init.h"

static void	install_daemon_binary(void)
{

}

static void	install_daemon_kernel_runtime(void)
{
	//printf("%s\n", daemon_init);
}

void		daemon_install(void)
{
	if (access("/usr/bin/Durex", F_OK) == -1)//file doesn't exist
		install_daemon_binary();
	if (access("/etc/sys.d/Durex", F_OK) == -1)//file doesn't exist
		install_daemon_kernel_runtime();
}
