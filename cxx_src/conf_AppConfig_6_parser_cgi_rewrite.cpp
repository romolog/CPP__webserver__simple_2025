/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_redirections.cpp     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:06:58 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

//	rewrite *.ext new_URI
void	AppConfig::parse_cgi_rewrite(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	ServerConfig& new_serv_conf)
{	

	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	std::string cgi_rewrite(it->content_str_);
	if (	cgi_rewrite.empty() || \
			cgi_rewrite.find_first_not_of(PATH_NAME_RADIX_) != std::string::npos || \
			cgi_rewrite[cgi_rewrite.size() - 1] == '-')
		throw AppException(prog_name_, "Conf file is invalid: CGI rewrite: not Posix portable filename charset: " + it->content_str_);

	new_serv_conf.cgi_rewrite_ = cgi_rewrite;

	to_next_no_content_lexem(it, it_end);
}

void	AppConfig::parse_cgi_rewrite(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& , \
	std::vector<int>& , \
	Location& )
{	
	throw AppException(prog_name_, "Conf file is invalid: CGI rewrite at 'Location' level: " + it->content_str_);

}