/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_serv_name.cpp        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:08:05 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

void	AppConfig::parse_upload(std::vector<Lexem>::iterator& , std::vector<Lexem>::iterator& , std::vector<int>&, ServerConfig& )
{	
	throw AppException(prog_name_, "Conf file is invalid: Upload_dir is inside 'Server' block");
}


void	AppConfig::parse_upload(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems , Location& new_location)
{	
	count_parsed_unique_lexem(LXM_UPLOAD_, parsed_lexems, it);
	to_next_content_lexem(it, it_end);

	if (it->content_str_ == "on")
		new_location.upload_allowed_ = YES_;
	else if (it->content_str_ == "off")
		new_location.upload_allowed_ = NO_;
	else
		throw AppException(prog_name_, "Conf file has invalid info: Autoindex value is invalid: " + it->content_str_);

	to_next_no_content_lexem(it, it_end);
}
