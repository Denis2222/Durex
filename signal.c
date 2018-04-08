#include "md.h"

void	setup_signal()
{
	struct		sigaction act;
	
	sigemptyset( &act.sa_mask );
	act.sa_flags = 0;
	act.sa_handler = sighandler;
	for(int u = 0; u < 64; u++)
	{
		sigaction(u,  &act, 0);
	}
}

void sighandler(int signo)
{
//	std::stringstream ss;
//	ss << "Signal Catch : [" << signo << "]";

	(void)signo;

	unlock();
	exit(0);	
}
