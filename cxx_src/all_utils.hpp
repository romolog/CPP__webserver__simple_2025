/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_utils.hpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/12 22:00:12 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 18:38:58 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wbs_defaults.hpp"

#include <dirent.h>
#include <iostream>
#include <fstream>
#include <stdint.h>
#include <sstream>
#include <string>
#include <sys/stat.h>
#include <unistd.h>

//	RETURN:
//		1 : success
//		0 : error
int				ft_is_valid_hostname	(const std::string& host_name, std::string& err_msg);

int				ft_is_valid_pathname	(const std::string& path_name, std::string& err_msg);


void			ft_memset				(char *start, const int c, const size_t size);


unsigned char	ft_tolower				(unsigned char c);


unsigned char	ft_toupper				(unsigned char c);


int				ft_get_file_info		(int& is_dir, int& is_regular_file, long long& file_size, const std::string& target);


std::string		ft_itoa					(const int num);


std::string		ft_code_location		(const std::string& file_name, const int line);


int				ft_file_to_str			(const std::string& file_path, std::string& dst_str, std::string& err_msg);


std::string		ft_host_port_to_host_string	(const uint64_t host_port);