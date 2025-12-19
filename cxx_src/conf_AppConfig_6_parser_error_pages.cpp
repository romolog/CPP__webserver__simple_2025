/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_error_pages.cpp      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:06:02 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "wbs_defaults.hpp"
#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

// error_page 123 ./err_page_file_name
// we know about special error codes > 599, but we do not allow them

void	AppConfig::parse_error_page(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	ServerConfig& new_serv_conf)
{	

	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	std::string err_num_str(it->content_str_);

	int err_num = atoi(err_num_str.c_str());
	if (err_num_str.empty() || \
		err_num_str.size() > 3 || \
		err_num_str.find_first_not_of(DIGIT_RADIX_) != std::string::npos || \
		err_num < 0 || \
		err_num > 599)
		throw AppException(prog_name_, "Conf file has invalid info: Invalid error code: " + err_num_str);

	to_next_content_lexem(it, it_end);

	std::map<int, std::string>::iterator it_map_low = new_serv_conf.err_pages_.lower_bound(err_num);
	std::map<int, std::string>::iterator it_map_end = new_serv_conf.err_pages_.end();

	if (it_map_low != it_map_end && it_map_low->first == err_num)
		throw AppException(prog_name_, "Conf file is invalid: Duplicated error codes: " + err_num_str);
	else
		new_serv_conf.err_pages_[err_num] = it->content_str_;

	to_next_no_content_lexem(it, it_end);
}

void	AppConfig::parse_error_page(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	Location& new_location)
{	
	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	std::string err_num_str(it->content_str_);

	int err_num = atoi(err_num_str.c_str());
	if (err_num_str.empty() || \
		err_num_str.size() > 3 || \
		err_num_str.find_first_not_of(DIGIT_RADIX_) != std::string::npos || \
		err_num < 0 || \
		err_num > 599)
		throw AppException(prog_name_, "Conf file has invalid info: Invalid error code: " + err_num_str);

	to_next_content_lexem(it, it_end);

	std::map<int, std::string>::iterator it_map_low = new_location.err_pages_.lower_bound(err_num);
	std::map<int, std::string>::iterator it_map_end = new_location.err_pages_.end();

	if (it_map_low != it_map_end && it_map_low->first == err_num)
		throw AppException(prog_name_, "Conf file has invalid info: Duplicated error codes: " + err_num_str);
	else
		new_location.err_pages_[err_num] = it->content_str_;

	to_next_no_content_lexem(it, it_end);
}