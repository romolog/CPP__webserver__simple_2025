/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_autoindex.cpp        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:04:34 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"
#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

void	AppConfig::parse_autoindex(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	ServerConfig& new_serv_conf)
{	

	count_parsed_unique_lexem(LXM_AUTOINDEX_, parsed_lexems, it);
	to_next_content_lexem(it, it_end);

	if (it->content_str_ == "on")
		new_serv_conf.autoindex_ = YES_;
	else if (it->content_str_ == "off")
		new_serv_conf.autoindex_ = NO_;
	else
		throw AppException(prog_name_, "Conf file has invalid info: Autoindex value is invalid: " + it->content_str_);

	to_next_no_content_lexem(it, it_end);
}

void	AppConfig::parse_autoindex(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	Location& new_location)
{	

	count_parsed_unique_lexem(LXM_AUTOINDEX_, parsed_lexems, it);
	to_next_content_lexem(it, it_end);

	if (it->content_str_ == "on")
		new_location.autoindex_ = YES_;
	else if (it->content_str_ == "off")
		new_location.autoindex_ = NO_;
	else
		throw AppException(prog_name_, "Conf file has invalid info: Autoindex value is invalid: " + it->content_str_);

	to_next_no_content_lexem(it, it_end);
}

