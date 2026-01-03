/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_root.cpp             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:07:20 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include <string>
#include <vector>
#include <cassert>

void	AppConfig::parse_root(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, ServerConfig& new_serv_conf)
{	
	count_parsed_unique_lexem(LXM_ROOT_, parsed_lexems, it);
	to_next_content_lexem(it, it_end);
	new_serv_conf.root_ = it->content_str_;
	to_next_no_content_lexem(it, it_end);
}

void	AppConfig::parse_root(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, Location& new_location)
{	
	count_parsed_unique_lexem(LXM_ROOT_, parsed_lexems, it);
	to_next_content_lexem(it, it_end);
	new_location.root_ = it->content_str_;

	to_next_no_content_lexem(it, it_end);
}