#ifndef TINTIN_REPORTER_HPP
# define TINTIN_REPORTER_HPP


# include <ctime>

class Tintin_reporter
{
	public:
		Tintin_reporter();
		Tintin_reporter(const Tintin_reporter&reporter);
		~Tintin_reporter();
		Tintin_reporter& operator=(const Tintin_reporter&reporter);

		void Log(char *str, e_logtype type);
		void Logstd(std::string str, e_logtype type);

	private:
		std::string filename;

};

#endif
