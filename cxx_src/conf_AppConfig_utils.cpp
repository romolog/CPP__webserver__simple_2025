/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_utils.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 19:43:04 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "all_AppException.hpp"

#include <string>
#include <vector>

int	AppConfig::to_next_content_lexem(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end)
{
	++it;
	if (it == it_end)
		throw AppException(prog_name_, "Conf file has invalid syntax: Incomplete" + (it - 1)->content_str_);
	if (it->type_ != LXM_A_CONTENT_)
		throw AppException(prog_name_, "Conf file has invalid syntax: Keyword instead of expected value field: " + (it - 1)->content_str_ + " " + it->content_str_ + " : 28");
	return (1);
}


int	AppConfig::to_next_no_content_lexem(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end)
{
	++it;
	if (it == it_end)
		throw AppException(prog_name_, "Conf file has invalid syntax: Incomplete" + (it - 1)->content_str_);
	if (it->type_ == LXM_A_CONTENT_)
		throw AppException(prog_name_, "Conf file has invalid syntax: Unsupported keyword or unexpected value field" + (it - 1)->content_str_ + " " + it->content_str_ + " : 39");
	return (1);
}


void	AppConfig::count_parsed_unique_lexem(int lexem_type, std::vector<int>& parsed_lexems, std::vector<Lexem>::iterator& it)
{
	++parsed_lexems[lexem_type];
	if (parsed_lexems[lexem_type] > 1)
		throw AppException(prog_name_, "Conf file has invalid syntax: Redundant field: " + it->content_str_);
}

