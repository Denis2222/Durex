#include "md.hpp"

int	islock()
{
	if( access( LOCK_PATH, F_OK ) != -1 ) {
		return (1);
	} else {
		return (0);
	}
}

void lock()
{
	int fd;
	fd = open(LOCK_PATH, O_RDWR|O_CREAT, 0777);
	if (fd != -1)
		close(fd);
}

void unlock()
{
	if (unlink(LOCK_PATH) != 0)
	{
		perror("unlink()");
	}
}

