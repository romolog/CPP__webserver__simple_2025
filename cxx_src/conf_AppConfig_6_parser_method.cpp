/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_method.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:06:47 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

void	AppConfig::parse_limit_except_method(	std::vector<Lexem>::iterator& it, \
												std::vector<Lexem>::iterator& , \
												std::vector<int>& , \
												ServerConfig& )
{	
	throw AppException(prog_name_, "Conf file is invalid: 'Limit except' is inside 'Server' block: " + it->content_str_ + ft_code_location(__FILE__, __LINE__));
}

void	AppConfig::parse_limit_except_method(	std::vector<Lexem>::iterator& it, \
												std::vector<Lexem>::iterator& it_end, \
												std::vector<int>& parsed_lexems, \
												Location& new_location)
{	
	count_parsed_unique_lexem(LXM_LIMIT_EXCEPT_, parsed_lexems, it);
	to_next_content_lexem(it, it_end);

	new_location.method_bits_ = 0;

	do 
	{
		int bit_shift = 	DELETE_ * (it->content_str_ == "DELETE") + \
							GET_ * (it->content_str_ == "GET") + \
							POST_ * (it->content_str_ == "POST");
		if (bit_shift)
			new_location.method_bits_ += (static_cast<uint32_t>(1) << bit_shift);
		else
			throw AppException(prog_name_, "Conf file is invalid: Method is invalid or unsupported: " + it->content_str_);
		

			
	} 
	while ((it + 1)->type_ == LXM_A_CONTENT_ && to_next_content_lexem(it, it_end));

	to_next_no_content_lexem(it, it_end);
}
