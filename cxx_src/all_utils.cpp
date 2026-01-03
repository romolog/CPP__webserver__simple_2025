/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_utils.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:00:12 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 18:32:17 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_utils.hpp"
#include <cerrno>
#include <cstring>

//	Hostnames are composed of a sequence of labels concatenated with dots. For example, "en.wikipedia.org" is a hostname. 
//	Each label must be 1 to 63 octets long.[2] The entire hostname, including the delimiting dots, has a maximum of 253 ASCII characters.[4]
//	The Internet standards (Request for Comments) for protocols specify that labels may contain only the ASCII letters a through z (in a case-insensitive manner), 
//	the digits 0 through 9, and the hyphen-minus character ('-'). The original specification of hostnames required that labels start with an alpha character and not end with a hyphen.[5] 
//	However, a subsequent specification permitted hostname labels to start with digits.


int		ft_is_valid_hostname(const std::string& host_name, std::string& err_msg)
{
	std::string::size_type host_name_len = host_name.size();
	if (host_name.empty() || host_name.find_first_not_of(HOSTNAME_RADIX_) != std::string::npos || host_name_len > 253 || host_name[host_name_len - 1] == '-')
	{
		err_msg = "Hostname is invalid: " + host_name;
		return (0);
	}

	std::string::size_type label_start = 0;
	std::string::size_type dot_pos = host_name.find('.', label_start);
	std::string::size_type label_len = 0;
	err_msg = "Hostname is invalid: Label size > 63: " + host_name;


	while(dot_pos != std::string::npos)
	{
		label_len = dot_pos - label_start;
		if (label_len > 63)
			return (0);
		label_start = dot_pos + 1;
		dot_pos = host_name.find('.', label_start);
	}


	label_len = host_name_len - host_name.rfind('.'); 
	if (label_len > 63)
		return (0);

	err_msg = "hostname is valid ";
	return (1);
}

//	RETURN:
//		1 : valid
//		0 : invalid
int		ft_is_valid_pathname(const std::string& path_name, std::string& err_msg)
{
	std::string::size_type path_name_len = path_name.size();
	if (	path_name.empty() || \
			path_name[0] != '/' || \
			path_name.find_first_not_of(PATH_NAME_RADIX_) != std::string::npos || \
			path_name[path_name_len - 1] == '-')
	{
		err_msg = "Path name is invalid: " + path_name;
		return (0);
	}

	return (1);
}


void	ft_memset(char *start, const int c, const size_t size)
{
	if (!start || !size || c < 0 || c > 127)
		return ;
	for (size_t i = 0; i < size; ++i)
		*(start + i) = c;
}

unsigned char	ft_toupper(unsigned char c)
{
	return std::toupper(c);
}


unsigned char	ft_tolower(unsigned char c)
{
	return std::tolower(c);
}

//	RETURN:
//		 0 : success
//		-1 : error (perm denied, size to large, doesn't exist, isn't a regular file or directory - maybe alias etc)
//	SET VARS:
//		regular file		: (is_regular file = 1, is_dir = 0, file_size = file.size)
//		directory 			: (is_regular file = 0, is_dir = 1, file_size = -1)
//		error				: (is_regular file = -1, is_dir = -1, file_size = -1)
int				ft_get_file_info(int& is_dir, int& is_regular_file, long long& file_size, const std::string& target)
{
	is_dir = -1;
	is_regular_file = -1;
	file_size = -1;

	struct stat file_info;
	if (stat(target.c_str(), &file_info))
	{
		// std::cerr << "ERNO : " << strerror(errno) << std::endl;
		return (-1);
	}
	else if ((file_info.st_mode & S_IFDIR))
	{
		is_dir = 1;
		is_regular_file = 0;
	}
	else if (file_info.st_mode & S_IFREG)
	{
		is_regular_file = 1;
		is_dir = 0;
		file_size = file_info.st_size;
	}
	else // file is not dir or regular file, could be alias etc.
		return (-1);
	
	return (0);	
}



std::string		ft_itoa(const int num)
{
	std::stringstream ss;
	ss << num;
	return (ss.str());	
}

// RETURN:
//		std::string "FILE: <file_name> : LINE: <line number>"
std::string		ft_code_location(const std::string& file_name, const int line)
{
	std::stringstream ss;
	ss << "\n\t\t\t: FILE: " << file_name << "\n\t\t\t: LINE: " << line;
	return (ss.str());	
}

// 	RETURN:
//		 0 : success
//		-1 : error
#include <filesystem>
int		ft_file_to_str(const std::string& file_path, std::string& dst_str, std::string& err_msg)
{
	std::ifstream ifs(file_path.c_str());
	if(!ifs.is_open())
	{
		err_msg = "Can't open file: " + file_path + ft_code_location(__FILE__, __LINE__);
		return (-1);
	}


	std::stringstream ss;
	ss << ifs.rdbuf();
	if (!ifs.good())
	{
		err_msg = "Can't read file: " + file_path + ft_code_location(__FILE__, __LINE__);
		return (-1);
	}

	dst_str = ss.str();
	return (0);
}

std::string		ft_host_port_to_host_string	(const uint64_t host_port)
{
	std::stringstream ss;
	uint32_t host = static_cast<uint32_t>(host_port >> 32);
	ss << (host >> 24) << '.' << ((host << 8) >> 24) << '.' << ((host << 16) >> 24) << '.' << ((host << 24) >> 24);

	return (ss.str());
}