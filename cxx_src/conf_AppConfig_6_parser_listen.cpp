/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_listen.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:13:38 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "wbs_defaults.hpp"
#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>

void	AppConfig::parse_listen(std::vector<Lexem>::iterator&, std::vector<Lexem>::iterator& , std::vector<int>& , Location&)
{	
	throw AppException(prog_name_, "Conf file has invalid info: Listen is inside location");
}


// listen 0.0.0.0:8000
void	AppConfig::parse_listen(std::vector<Lexem>::iterator& it, \
									std::vector<Lexem>::iterator& it_end, \
									std::vector<int>& parsed_lexems, \
									ServerConfig& new_serv_conf)
{	
	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	// GET HOST and PORT_STRING
	uint64_t host = DEFAULT_HOST_;
	std::string::size_type host_end = it->content_str_.find(':');
	std::string port_str; 


	if (host_end != std::string::npos)
	{
		port_str = it->content_str_.substr(host_end + 1, std::string::npos);
		host = get_host(host_end, it);
	}
	else
		port_str = it->content_str_;

	uint64_t port = get_port(port_str, it);

	uint64_t host_port = (host << 32) + port;

	//	if unique HOST-PORT - add to application map 'app_host_ports_', otherwise ERROR
	if (!app_host_ports_.count(host_port))
		app_host_ports_.insert(host_port);
	else
		throw AppException(prog_name_, "Conf file is invalid: Host-port is duplicated for port: " + port_str);		

	new_serv_conf.host_ports_.push_back(host_port);
	
	to_next_no_content_lexem(it, it_end);
}


u_int32_t	AppConfig::get_host(std::string::size_type host_end, std::vector<Lexem>::iterator& it)
{
	if (it->content_str_.find(host_end, ':') != std::string::npos)
		throw AppException(prog_name_, "Conf file has invalid info: Host is invalid or not supported IPv6: " + it->content_str_);
	
	uint32_t host = 0;
	
	std::string host_str = it->content_str_.substr(0, host_end);

	std::string::size_type seg_start = 0;
	std::string::size_type seg_end = host_str.find('.');
	int segments = 0;
	for(; segments < 4 && seg_end != std::string::npos; ++segments)
	{
		std::string seg_str = host_str.substr(seg_start, seg_end - seg_start);
		int seg = atoi(seg_str.c_str());
		if (seg_str.empty() || \
			seg_str.size() > 3 || \
			seg_str.find_first_not_of(DIGIT_RADIX_) != std::string::npos || \
			seg < 0 || \
			seg > 255)
			throw AppException(prog_name_, "Conf file has invalid info: Host is invalid: " + it->content_str_);
		
		host = (host << 8) + seg;

		seg_start = seg_end + 1;
		if (segments < 2) 
			seg_end = host_str.find('.', seg_start);
		else
			seg_end = host_end;
	}
	if (segments != 4)
		throw AppException(prog_name_, "Conf file has invalid info: Host is invalid: " + it->content_str_);

	return (host);
}


u_int16_t	AppConfig::get_port(std::string& port_str, std::vector<Lexem>::iterator& it)
{

	if (port_str.empty() || \
		port_str.size() > 5 || \
		port_str.find_first_not_of(DIGIT_RADIX_) != std::string::npos)
		throw AppException(prog_name_, "Conf file has invalid info: Listen to: " + it->content_str_);
	
	int port = atoi(port_str.c_str());
	if (port < 0 || port > __UINT16_MAX__)
		throw AppException(prog_name_, "Conf file has invalid info: Listen to: " + it->content_str_);
	
	return (port);
}



