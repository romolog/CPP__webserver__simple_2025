/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_4_location_start.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:04:02 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>

#define BLANK_RADIX_ " \f\r\t\v"
#define COMMENT_RADIX_ "#;"
#define DIGIT_RADIX "0123456789"
#define MIN_TOKENS_NUM_ 11
#define SPACE_RADIX_ " \f\n\r\t\v"

#define REQUIRED_LEXEMS_ 3


//	if called inside 'location' throws exception
void	AppConfig::parse_location(std::vector<Lexem>::iterator&, std::vector<Lexem>::iterator&, std::vector<int>&, Location&)
{
	throw AppException(prog_name_, "Conf file has invalid syntax: Location is inside location" + ft_code_location(__FILE__, __LINE__));
}

//	expected call inside 'server'
void	AppConfig::parse_location(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	ServerConfig& new_serv_conf)
{	
	// exception_if_inside_location(it, parsed_lexems);
	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	std::vector<int> parsed_location_lexems(NUM_LEX_TYPES_, 0);

	Location new_location;
	new_location.path_ = get_location_path(it->content_str_);

	to_next_no_content_lexem(it, it_end);
	parse_brace_open(it, it_end, parsed_location_lexems, new_serv_conf);

	LocationLexemParserPtr location_lexem_parsers[NUM_LEX_TYPES_] = \
	{
			&AppConfig::throw_exception,
		&AppConfig::parse_autoindex,
		&AppConfig::parse_brace_open,
		&AppConfig::parse_brace_close,
		&AppConfig::parse_cgi,
		&AppConfig::parse_cgi_rewrite,
		&AppConfig::parse_client_max_body,
		&AppConfig::parse_error_page,
		&AppConfig::parse_index_page,
		&AppConfig::parse_limit_except_method,
		&AppConfig::parse_listen,
		&AppConfig::parse_location,
		&AppConfig::parse_redirections,
		&AppConfig::parse_root,
		&AppConfig::parse_server_name,
		&AppConfig::parse_upload,		
			&AppConfig::throw_exception
	};

	while (it != it_end && !parsed_location_lexems[LXM_BRACE_CLOSE_])
		(*this.*location_lexem_parsers[it->type_])(it, it_end, parsed_location_lexems, new_location);

	if (!parsed_location_lexems[LXM_BRACE_CLOSE_] || parsed_location_lexems[LXM_BRACE_OPEN_] > 1 || parsed_location_lexems[LXM_BRACE_OPEN_] != parsed_location_lexems[LXM_BRACE_CLOSE_])	
		throw AppException(prog_name_, "Conf file has invalid syntax: Invalid braces" + ft_code_location(__FILE__, __LINE__));

	set_location_defaults(parsed_location_lexems, new_location);

	// no obligatory at the moment
	//	check if obligatory fields are set
	// check_location_obligatories(parsed_location_lexems);

	//	check if unique fields are set only once
	//	check if non-location fields are empty
	check_location_redundants(parsed_location_lexems);

	// decided that all fields are compatible
	// check_location_noncompatible_fields(parsed_location_lexems);
	
	new_serv_conf.locations_.insert(std::pair<std::string, Location>(new_location.path_, new_location));
}

std::string&	AppConfig::get_location_path(std::string& path_str)
{
	std::string err_msg;
	if (!ft_is_valid_pathname(path_str, err_msg))
		throw AppException(prog_name_, "Conf file is invalid: " + err_msg + ft_code_location(__FILE__, __LINE__));

	return (path_str);
}


void	AppConfig::set_location_defaults(std::vector<int>& parsed_location_lexems, Location& new_location)
{
	if (!parsed_location_lexems[LXM_INDEX_PAGE_])
	{
		++parsed_location_lexems[LXM_INDEX_PAGE_];
		new_location.index_page_ = DEFAULT_INDEX_PAGE_;
	}
}

// void	AppConfig::check_location_obligatories(std::vector<int>& parsed_location_lexems)
// {
	// no obligatory fields at the moment
// }

void	AppConfig::check_location_redundants(std::vector<int>& parsed_location_lexems)
{
	//	check if unique fields are unique
	int unique_location_fields[] = {LXM_AUTOINDEX_, LXM_CLIENT_MAX_BODY_, LXM_INDEX_PAGE_, LXM_ROOT_};
	int len = sizeof(unique_location_fields) / sizeof(unique_location_fields[0]);
	for(int i = 0; i < len; ++i)
		if (parsed_location_lexems[unique_location_fields[i]] > 1)
			throw AppException(prog_name_, "Conf file has invalid syntax: Redundant field" + ft_code_location(__FILE__, __LINE__));

	//	check if non-location fields are empty
	int empty_location_fields[] = {LXM_LISTEN_, LXM_LOCATION_, LXM_SERVER_NAME_};
	len = sizeof(empty_location_fields) / sizeof(empty_location_fields[0]);
	for(int i = 0; i < len; ++i)
		if (parsed_location_lexems[empty_location_fields[i]])
			throw AppException(prog_name_, "Conf file has invalid syntax: Location field is inside server part" + ft_code_location(__FILE__, __LINE__));
}

// void	AppConfig::check_location_noncompatible_fields(std::vector<int>& parsed_location_lexems)
// {
// }