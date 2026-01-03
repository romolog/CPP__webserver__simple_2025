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
		std::cout << "CGI: script runs: use env: FAIL env not found >>||<<\n";
		return (0);		
	}

	std::string method("FAIL");
	int mthd = 0;

	while (ptr && *ptr)
	{
		std::string env_str(*ptr); 
		if (!env_str.find("REQUEST_METHOD="))
		{
			method = env_str;
			mthd = 1;
		}
			
		++ptr;

		if (mthd)
			break;;
	}

	std::cout << "CGI: script runs: REQUEST METHOD = >>|" << method << "|<<\n";
	
	return (0);
}