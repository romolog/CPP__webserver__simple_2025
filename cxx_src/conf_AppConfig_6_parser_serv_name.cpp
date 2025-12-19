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

void	AppConfig::parse_server_name(std::vector<Lexem>::iterator& , std::vector<Lexem>::iterator& , std::vector<int>& , Location& )
{	
	throw AppException(prog_name_, "Conf file is invalid: Server_name is inside 'Location' block");
}

void	AppConfig::parse_server_name(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, ServerConfig& new_serv_conf)
{	
	count_parsed_unique_lexem(LXM_SERVER_NAME_, parsed_lexems, it);
	to_next_content_lexem(it, it_end);

	std::transform(it->content_str_.begin(), it->content_str_.end(), it->content_str_.begin(), ft_tolower);
	new_serv_conf.server_name_ = it->content_str_;

	to_next_no_content_lexem(it, it_end);
}
