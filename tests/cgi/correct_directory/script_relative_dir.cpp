#include <stdexcept>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

int main(void)
{

	std::ifstream ifs("cgi_relative_dir_source.file");
	if (!ifs.is_open())
		throw std::runtime_error("CGI script: Can't open source file\n");

	std::stringstream ss;
	ss << ifs.rdbuf();
	if (!ifs.good())
		throw std::runtime_error("CGI script: Can't read source file\n");
	std::string msg = ss.str();
	
	if (msg.empty())
		throw std::runtime_error("CGI script: Source file is empty");

	std::cout << \
	"Content-type: txt/html\n\n" << \
	"CGI: script runs: " << msg << \
	std::endl;

	// std::cout << "\n\t\tCGI: script runs: " << msg << '\n';
	
	return (0);
}