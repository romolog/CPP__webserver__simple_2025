/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_16_assign_location_to_request.cpp      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:56:24 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"
#include "all_utils.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>


//	After Request-line and Headers are read 
//		- we read URI and got: PATH
//		- we read HEADER 'Host" and got: HOSTNAME
//		- we also have 'host-port' 
//	Now we have enough data to assign a configuration-LOCATION to this request
//		- check server_name == HOSTNAME, othewise set error-response 400 BAD REQUEST
//		- find a location : location.path matching request.uri_path

//	RETURN:
//			 1 : not ready to assign
//			 0 : success, already assigned
//			-1 : error
//	Does NOT check, if URI and HEADERS are already parsed
int		AppRun::assign_location_to_request(Handler* handler_ptr)
{
	// IMPORTANT: this fucntion can go only after URI_PARSED and HEADERS_PARSED

	ServerConfig& serv_config = handler_ptr->config_server_;
	HTTP_Request& request = handler_ptr->http_request_;

	if (request.is_parsed_assigned_location_ == YES_)
		return (0);
	else if (request.is_parsed_headers_ == NO_)
		return (1);

	std::string serv_name = serv_config.server_name_;
	std::string host_name = request.header_hostname_;
	std::string host_port_str = ft_host_port_to_host_string(handler_ptr->host_port_);

	if (host_name != serv_name && host_name != host_port_str)
	{
		set_error_response(	handler_ptr, "Server_name doesn't match request header 'HOST' host_name: " + serv_name + " vs " + host_name + ft_code_location(__FILE__, __LINE__), 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}

	std::map<std::string, Location>& locations = serv_config.locations_;

	std::string& uri_path = request.uri_path_; 

	std::map<std::string, Location>::iterator it = locations.begin();
	std::map<std::string, Location>::iterator it_end = locations.end();

	std::string match;
	for(; it != it_end; ++it)
	{
		if (uri_path.find(it->first) != std::string::npos && it->first.size() > match.size())
			match = it->first;
	}

	if (locations.count(match))
		handler_ptr->config_location_ = locations[match];
	else
	{
		set_error_response(handler_ptr, "URI path doesn't match any LOCATION: " + handler_ptr->http_request_.uri_path_ + ft_code_location(__FILE__, __LINE__), 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}

	// check for METHOD-LIMIT
	uint32_t methods_allowed = handler_ptr->config_location_.method_bits_;
	if ( !(methods_allowed & (static_cast<uint32_t>(1) << request.method_)))
	{
		// 405 METHOD NOT ALLOWED
		set_error_response(	handler_ptr, "Request method is not allowed at matching 'Location'" + ft_code_location(__FILE__, __LINE__), 405);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}

	//	if request is CGI at server level - it remains CGI
	//	if request is not-CGI at server level, check if inside LOCATION request becomes CGI
	if (request.is_cgi_ == NO_)
	{
		std::string cgi_location_ext = handler_ptr->config_location_.cgi_ext_;
		std::string uri_ext = request.uri_ext_;
		
		//	got CGI STATUS
		if ( !uri_ext.empty() && uri_ext == cgi_location_ext)
			request.is_cgi_ = YES_;
	}

	// got REDIRECT status, IF NOT YET REDIRECTED ALREADY at Server level
	if (	handler_ptr->http_request_.redirection_code_ == NOT_SET_ && 
			handler_ptr->config_location_.redirection_code_ != NOT_SET_)
	{
		handler_ptr->http_request_.redirection_code_ = handler_ptr->config_location_.redirection_code_;
		handler_ptr->http_request_.redirection_path_ = handler_ptr->config_location_.redirection_path_;
	}

	request.is_parsed_assigned_location_ = YES_;

	return (0);
}
