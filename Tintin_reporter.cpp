#include "md.hpp"
#include "Tintin_reporter.hpp"

Tintin_reporter::Tintin_reporter()
{
	this->filename = "";
	struct stat st = {0};

	if (stat(LOG_FOLDER, &st) == -1) {
		mkdir(LOG_FOLDER, 0700);
	}
}

Tintin_reporter::Tintin_reporter(const Tintin_reporter&reporter)
{

	*this = reporter;
	this->filename.push_back('r');
//	std::cout << "new reporter from copy !" << std::endl;
	std::cout << this->filename << std::endl;
}

Tintin_reporter::~Tintin_reporter()
{
//	std::cout << "Delete reporter !" << std::endl;
}

Tintin_reporter& Tintin_reporter::operator=(const Tintin_reporter&reporter)
{
	if (this != &reporter)
	{
		this->filename = reporter.filename;
	}
	return (*this);
}

void Tintin_reporter::Logstd(std::string str, e_logtype type)
{
	time_t rawtime;
	struct tm * timeinfo;
	char buffer [100];

	time (&rawtime);
	timeinfo = localtime (&rawtime);

	// [ DD / MM / YYYY - HH : MM : SS])
	strftime (buffer,100,"[%d/%m/%Y - %H:%M:%S] ",timeinfo);

	int fd;

	fd = open(LOG_PATH, O_RDWR|O_CREAT|O_APPEND, 0777);
	if (fd != -1)
	{
		flock(fd, LOCK_EX);
		if (type == INFO)
			dprintf(fd, "%s %s %s\n", buffer,"[INFO]", str.c_str());
		else if (type == LOG)
			dprintf(fd, "%s %s User Input : %s\n", buffer,"[LOG]", str.c_str());
		else
			dprintf(fd, "%s %s %s\n", buffer,"[ERROR]", str.c_str());
		close(fd);
		flock(fd, LOCK_UN);
	}
}

void Tintin_reporter::Log(char *str, e_logtype type)
{
	std::string chars(str);

	Logstd(chars, type);
}
