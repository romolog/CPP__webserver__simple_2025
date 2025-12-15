#include <unistd.h>
#include <iostream>

int main(void)
{
	std::cout << "Content-type: txt/html\n\n";
	
	while (true)
	{
		sleep(10);
		std::cout << "CGI: script runs: infinite loop\n";
	};

	return (0);
}