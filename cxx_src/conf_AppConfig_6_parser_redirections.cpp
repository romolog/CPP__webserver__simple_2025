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
#include "all_utils.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

//	HTTP redirection, 
//		also known as URL forwarding or redirection, is a web development technique 
//		where a server sends a special response to a client's browser, 
//		instructing it to go to a different URL than initially requested. 
//		This is commonly used to manage content changes, such as moving a page to a new location, 
//		updating a domain name, or handling temporary website maintenance. 
//	Here's a more detailed explanation:
//		How it works:
//			When a user requests a URL, the server can respond with a "redirect" status code (starting with 3) and a "Location" header containing the new URL. 
//		Purpose:
//			Redirects are used to:
//				Maintain links: When content is moved, redirects ensure that old links and bookmarks still work, sending users to the new location. 
//				Handle site changes: Redirects are crucial when changing domain names or merging websites. 
//				Manage temporary unavailability: Redirects can be used during site maintenance or downtime to inform users of the situation. 
//				Improve user experience: Redirects prevent users from encountering broken links or error pages. 
//		Types of redirects:
//			301 (Moved Permanently): Indicates a permanent move to a new URL. 
//			302 (Found/Moved Temporarily): Indicates a temporary move. 
//		Benefits:
//			Preserves SEO: Proper redirection helps search engines understand content changes and avoid ranking issues. 
//			Seamless user experience: Users are automatically redirected to the new location without needing to manually update the URL. 
//			Simplified link management: Redirects allow for easy management of affiliate links or tracking codes. 

//	 IMPORTANT: README: 
// https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Redirections

//	 IMPORTANT: README: RFC: 
// https://www.rfc-editor.org/rfc/rfc9110#status.3xx


//	return 3xx new_URI_path

void	AppConfig::parse_redirections(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	ServerConfig& new_serv_conf)
{	
	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	std::string redir_code_str(it->content_str_);
	
	if (redir_code_str.size() != 3 || \
		redir_code_str[0] != '3' || \
		redir_code_str.find_first_not_of(DIGIT_RADIX_) != std::string::npos)
		throw AppException(prog_name_, "Conf file is invalid: Redirection status code invalid: " + redir_code_str);

	new_serv_conf.redirection_code_ = std::atoi(redir_code_str.c_str());

	to_next_content_lexem(it, it_end);

	std::string redir_path(it->content_str_);
	std::string err_msg;
	if (!ft_is_valid_pathname(redir_path, err_msg))
		throw AppException(prog_name_, err_msg);

	new_serv_conf.redirection_path_ = redir_path;

	to_next_no_content_lexem(it, it_end);
}

void	AppConfig::parse_redirections(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	Location& new_location)
{	
	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	std::string redir_code_str(it->content_str_);
	
	if (redir_code_str.size() != 3 || \
		redir_code_str[0] != '3' || \
		redir_code_str.find_first_not_of(DIGIT_RADIX_) != std::string::npos)
		throw AppException(prog_name_, "Conf file is invalid: Redirection status code invalid: " + redir_code_str);

	new_location.redirection_code_ = std::atoi(redir_code_str.c_str());

	to_next_content_lexem(it, it_end);

	std::string redir_path(it->content_str_);
	std::string err_msg;
	if (!ft_is_valid_pathname(redir_path, err_msg))
		throw AppException(prog_name_, err_msg);

	new_location.redirection_path_ = redir_path;

	to_next_no_content_lexem(it, it_end);
}