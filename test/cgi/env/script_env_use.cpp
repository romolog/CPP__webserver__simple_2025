#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(int , char** , char** env)
{
	char** ptr = env;

	std::cout << "Content-type: txt/html\n\n";

	if (!ptr)
	{
		std::cout << "CGI: script runs: use env: FAIL env not found\n";
		return (0);		
	}

	int usr = 0;
	int oldpwd = 0;
	int pwd = 0;
	int mthd = 0;
	int pid = 0;

	while (ptr && *ptr)
	{
		std::string env_str(*ptr); 
		if (!env_str.find("USER="))
		{
			std::cout << "CGI: script runs: use env: " << env_str << '\n';
			usr = 1;
		}
		else if (!env_str.find("OLDPWD="))
		{
			std::cout << "CGI: script runs: use env: " << env_str << '\n';
			oldpwd = 1;
		}
		else if (!env_str.find("PWD="))
		{
			std::cout << "CGI: script runs: use env: " << env_str << '\n';
			pwd = 1;
		}
		else if (!env_str.find("REQUEST_METHOD="))
		{
			std::cout << "CGI: script runs: use env: " << env_str << '\n';
			mthd = 1;
		}
		else if (!env_str.find("SYSTEMD_EXEC_PID="))
		{
			std::cout << "CGI: script runs: use env: " << env_str << '\n';
			pid = 1;
		}
			
		++ptr;

		if (usr * oldpwd * pwd * mthd * pid)
			return (EXIT_SUCCESS);
	}

	std::cout << "CGI: script runs: use env: FAIL some vars are not found\n";
	
	return (0);
}