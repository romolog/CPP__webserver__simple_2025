/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_braces.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:05:15 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"
#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

void	AppConfig::parse_brace(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, int brace_type)
{
	++parsed_lexems[brace_type];
	++it;
	if (parsed_lexems[LXM_BRACE_OPEN_] > 1)
		throw AppException(prog_name_, "Conf file has invalid syntax: Invalid braces: 25");
	if (parsed_lexems[LXM_BRACE_CLOSE_])
		return ;
		
	if (it == it_end)
		return ;
	else if (it->type_ == LXM_BRACE_OPEN_ || \
			it->type_ == LXM_BRACE_CLOSE_ || \
			it->type_ == LXM_A_CONTENT_)
		throw AppException(prog_name_, "Conf file has invalid syntax: " + (it - 1)->content_str_ + " " + it->content_str_);	
}

void	AppConfig::parse_brace_open(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, ServerConfig&)
{
	//	first called in "expected mode" at the beginning of parsing
	if (it->type_ != LXM_BRACE_OPEN_)
		throw AppException(prog_name_, "Conf file has invalid syntax: Invalid braces: 42");
	parse_brace(it, it_end, parsed_lexems, LXM_BRACE_OPEN_);
}

void	AppConfig::parse_brace_open(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, Location&)
{	
	//	first called in "expected mode" at the beginning of parsing	
	if (it->type_ != LXM_BRACE_OPEN_)
		throw AppException(prog_name_, "Conf file has invalid syntax: Invalid braces: 50");
	parse_brace(it, it_end, parsed_lexems, LXM_BRACE_OPEN_);
}


void	AppConfig::parse_brace_close(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, ServerConfig&)
{	
	if (it->type_ != LXM_BRACE_CLOSE_)
		throw AppException(prog_name_, "Conf file has invalid syntax: Invalid braces: 58");
	parse_brace(it, it_end, parsed_lexems, LXM_BRACE_CLOSE_);
}

void	AppConfig::parse_brace_close(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, Location&)
{	
	if (it->type_ != LXM_BRACE_CLOSE_)
		throw AppException(prog_name_, "Conf file has invalid syntax: Invalid braces: 65");
	parse_brace(it, it_end, parsed_lexems, LXM_BRACE_CLOSE_);
}