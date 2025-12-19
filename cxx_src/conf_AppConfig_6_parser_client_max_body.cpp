/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_client_body_buff.cpp :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:05:51 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "wbs_defaults.hpp"
#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>


void	AppConfig::parse_client_max_body(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	ServerConfig& new_serv_conf)
{	
	count_parsed_unique_lexem(it->type_, parsed_lexems, it);

	to_next_content_lexem(it, it_end);

	long long client_b_size = atoll(it->content_str_.c_str());
	if (it->content_str_.empty() || \
		it->content_str_.size() > 11 || \
		it->content_str_.find_first_not_of(DIGIT_RADIX_) != std::string::npos || \
		client_b_size > REQUEST_MAX_BODY_SIZE_ || \
		client_b_size < 0)
		throw AppException(prog_name_, "Conf file is invalid: Client max body size is invalid: " + it->content_str_ + " : Limit for max body size = " + REQUEST_MAX_BODY_SIZE_STR_);
		
	new_serv_conf.client_max_body_size_ = static_cast<int>(client_b_size);

	to_next_no_content_lexem(it, it_end);
}

void	AppConfig::parse_client_max_body(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	Location& new_location)
{	
	count_parsed_unique_lexem(it->type_, parsed_lexems, it);

	to_next_content_lexem(it, it_end);

	long long client_b_size = atoll(it->content_str_.c_str());
	if (it->content_str_.empty() || \
		it->content_str_.size() > 11 || \
		it->content_str_.find_first_not_of(DIGIT_RADIX_) != std::string::npos || \
		client_b_size > REQUEST_MAX_BODY_SIZE_ || \
		client_b_size < 0)
		throw AppException(prog_name_, "Conf file is invalid: Client max body size is invalid: " + it->content_str_ + " : Limit for max body size = " + REQUEST_MAX_BODY_SIZE_STR_);
		
	new_location.client_max_body_size_ = static_cast<int>(client_b_size);

	to_next_no_content_lexem(it, it_end);
}
