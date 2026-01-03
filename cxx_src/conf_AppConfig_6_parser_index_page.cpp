/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_index_page.cpp       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:06:15 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_AppException.hpp"
#include "conf_AppConfig.hpp"

#include <string>
#include <vector>
#include <cassert>

void	AppConfig::parse_index_page(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& , std::vector<int>& , ServerConfig& )
{	
	throw AppException(prog_name_, "Config invalid: Index page at 'Server' level: " + it->content_str_ \
						+ " " + (it + 1)->content_str_ + ft_code_location(__FILE__, __LINE__));
}

void	AppConfig::parse_index_page(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, Location& new_location)
{	

	count_parsed_unique_lexem(LXM_INDEX_PAGE_, parsed_lexems, it);
	to_next_content_lexem(it, it_end);
	new_location.index_page_ = it->content_str_;
	to_next_no_content_lexem(it, it_end);
}
