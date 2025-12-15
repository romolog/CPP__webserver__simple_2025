
#include <iostream>

int main(void)
{

	std::cout << "Content-type: txt/html\n\n";
	std::cout << "CGI: script runs: generates chunks for BUFF = 512:\n";

	int buff_size = 512;

	while (buff_size--)
		std::cout << "X";

	std::cout << '\n';

	return (0);
}