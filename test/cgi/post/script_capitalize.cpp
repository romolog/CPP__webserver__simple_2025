#include <algorithm>
#include <iostream>
#include <string>

static unsigned char	ft_toupper(unsigned char c)
{
	return std::toupper(c);
}

int main(void)
{


	std::string buff;
	std::string input;
	while (std::getline(std::cin, buff))
		input += buff;

	std::transform(input.begin(), input.end(), input.begin(), ft_toupper);

	std::cout << "Content-type: txt/html\n\n";
	std::cout << "\t\tCGI: script runs: capitalize:\n" << input << '\n';

	return (0);
}