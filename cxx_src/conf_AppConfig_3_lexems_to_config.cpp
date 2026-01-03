/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_3_config_start.cpp             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:02:56 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>

//	parse all lexems
//	the minimum parsing block is 'server{...}'
//	each parsed block is converted into t_config_map 'parsed_server_config_map'
//	and it is added to the FINAL t_config_map 'app_config'
void	AppConfig::lexems_to_config(void)
{
	std::vector<Lexem>::iterator it = lexems_.begin();
	std::vector<Lexem>::iterator it_end = lexems_.end();

	while (it != it_end)
	{
		t_config_map parsed_server_config_map = parse_server_config(it, it_end);
		app_config_.insert(parsed_server_config_map.begin(), parsed_server_config_map.end());
	}

}

t_config_map	AppConfig::parse_server_config(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end)
{
	//	parsing 1st lexem: expect it to be a keyword 'server'
	if (it->type_ != LXM_Z_SERVER_)
		throw AppException(prog_name_, "Conf file has invalid syntax: " + it->content_str_);
	++it;

	//	counter for parsed lexems: any time a lexem is paresed - increment lexem[TYPE]
	std::vector<int> parsed_lexems(NUM_LEX_TYPES_, 0);

	//	server parameters: everything excluding HOST, PORT and SERVER_NAME
	ServerConfig new_serv_conf;
	
	//	parse 2nd lexem: expect it to be a '{'
	parse_brace_open(it, it_end, parsed_lexems, new_serv_conf);

	ServerLexemParserPtr server_lexem_parsers[NUM_LEX_TYPES_] = \
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
	

	//	parsing all lexems in a while-loop, calling proper parser per lexem: 
	//		while (have Lexems && this server brases are not closed)
	//			parsers[index] : index = Lexem.type_
	//				"location" has its own internal parsing
	//				within "server" we parse location by one step calling "parse_location"
	while (it != it_end && !parsed_lexems[LXM_BRACE_CLOSE_])
		(*this.*server_lexem_parsers[it->type_])(it, it_end, parsed_lexems, new_serv_conf);


	if (!parsed_lexems[LXM_BRACE_CLOSE_] || parsed_lexems[LXM_BRACE_OPEN_] > 1 || parsed_lexems[LXM_BRACE_OPEN_] != parsed_lexems[LXM_BRACE_CLOSE_])
		throw AppException(prog_name_, "Conf file has invalid syntax: Invalid braces");

	//	set default values for missing fields
	set_server_defaults(parsed_lexems, new_serv_conf);

	//	check if obligatory fields are set	
	check_server_obligatories(parsed_lexems);

	//	check if unique fields are set only once
	//	check if non-server fields are empty
	check_server_redundants(parsed_lexems);

	//	decided that all fields are compatible
	//	check if non-compatible fields do not present at the same time
	// check_server_noncompatible_fields(parsed_lexems);


	//	make all combinations of host-port-servname-servconfig
	//	add them to the 'this_server_config_map'
	//	next step (outside this funciton) this 'this_server_config_map' will be added to the FINAL app_config_map
	t_config_map this_server_config_map = generate_config_map(new_serv_conf);

	return (this_server_config_map);
}


void	AppConfig::set_server_defaults(std::vector<int>& parsed_lexems, ServerConfig& new_serv_conf)
{
	if (!parsed_lexems[LXM_SERVER_NAME_])
	{
		++parsed_lexems[LXM_SERVER_NAME_];
		new_serv_conf.server_name_ = DEFAULT_SERVER_NAME_;
	}
	
	if (!parsed_lexems[LXM_LISTEN_])
	{
		++parsed_lexems[LXM_LISTEN_];
		new_serv_conf.host_ports_.push_back((static_cast<uint64_t>(DEFAULT_HOST_) << 32) + DEFAULT_PORT_);
	}
}

void	AppConfig::check_server_obligatories(std::vector<int>& parsed_lexems)
{
	//	check if obligatory fields are set
	int obligatory_server_fields[] = {LXM_LISTEN_, LXM_LOCATION_, LXM_SERVER_NAME_};
	int len = sizeof(obligatory_server_fields) / sizeof(obligatory_server_fields[0]);
	for(int i = 0; i < len; ++i)
		if (!parsed_lexems[obligatory_server_fields[i]])
			throw AppException(prog_name_, "Conf file is invalid: Obligatory field is missing");
}

// void	AppConfig::check_server_noncompatible_fields(std::vector<int>& parsed_lexems)
// {
// 	// nothing to do at the moment
// }

void	AppConfig::check_server_redundants(std::vector<int>& parsed_lexems)
{
	//	check if unique fields are unique
	int unique_server_fields[] = {LXM_AUTOINDEX_, LXM_CLIENT_MAX_BODY_, LXM_INDEX_PAGE_, LXM_ROOT_};
	int len = sizeof(unique_server_fields) / sizeof(unique_server_fields[0]);
	for(int i = 0; i < len; ++i)
		if (parsed_lexems[unique_server_fields[i]] > 1)
			throw AppException(prog_name_, "Conf file is invalid: Redundant field");

	//	check if non-server fields are empty
	int empty_server_fields[] = {LXM_LIMIT_EXCEPT_};
	len = sizeof(empty_server_fields) / sizeof(empty_server_fields[0]);
	for(int i = 0; i < len; ++i)
		if (parsed_lexems[empty_server_fields[i]])
			throw AppException(prog_name_, "Conf file is invalid: 'Location' field is inside 'Server' block");
}


t_config_map	AppConfig::generate_config_map(ServerConfig& serv_conf)
{
	t_config_map this_server_config_map;

	//	PARSE ALL HOST-PORT PAIRS:
	std::vector<uint64_t>::iterator host_port_it = serv_conf.host_ports_.begin();
	std::vector<uint64_t>::iterator host_port_it_end = serv_conf.host_ports_.end();	
	for (; host_port_it != host_port_it_end; ++host_port_it)
	{
		// COMBINE (HOST-PORT-SERV_CONF) and ADD IT TO THE MAP

		if (!app_config_.count(*host_port_it))
		{
			// INSERT TO THE MAP
			this_server_config_map.insert(std::pair<uint64_t, ServerConfig>(*host_port_it, serv_conf));
		}
		else
		{
			//	we already have the same HOST-PORT for some other config
			throw AppException(prog_name_, "Conf file is invalid: Duplicated HOST-PORT");
		}
	}
	return (this_server_config_map);
}