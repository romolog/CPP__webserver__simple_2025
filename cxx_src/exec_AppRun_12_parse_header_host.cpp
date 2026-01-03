/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_12_parse_header_host.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 15:22:21 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"
#include "all_utils.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>


// https://practical-scheme.net/gauche/man/gauche-refe/URI-parsing-and-construction.html
// https://www.gnu.org/software/emacs/manual/html_node/url/URI-Parsing.html


// https://www.rfc-editor.org/rfc/rfc9112#name-reconstructing-the-target-u
//	When making a request directly to an origin server, other than a CONNECT or server-wide OPTIONS request (as detailed below), 
//	a client MUST send only the absolute path and query components of the target URI as the request-target. 
//	If the target URI's path component is empty, the client MUST send "/" as the path within the origin-form of request-target. 
//	A Host header field is also sent, as defined in Section 7.2 of [HTTP]


//	RETURN:  
//			 1 : not ready to parse
//			 0 : already parsed
//			 0 : success
//			-1 : error
int		AppRun::parse_header_host(Handler* handler_ptr)
{
	HTTP_Request& request = handler_ptr->http_request_;

	if (request.is_parsed_headers_ == NO_)
		return (1);
	else if (request.is_parsed_header_host_ == YES_)
		return (0);
	else
		request.is_parsed_header_host_ = YES_;	

	//	GET /pub/WWW/TheProject.html HTTP/1.1
	//	Host: www.example.org:8080
	
	std::string header_host_body = handler_ptr->http_request_.headers_["HOST"].second;
	std::string::size_type colon_pos = header_host_body.find(':');
	std::string header_hostname = header_host_body.substr(0, colon_pos);
	std::string	header_port_str = header_host_body.erase(0, colon_pos + 1);

	std::string err_msg;
	int check_hostname = ft_is_valid_hostname(header_hostname, err_msg);
	if (!check_hostname)
	{
		set_error_response(handler_ptr, err_msg + ft_code_location(__FILE__, __LINE__), 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}


	if (!handler_ptr->http_request_.uri_hostname_.empty())
	{
		if (handler_ptr->http_request_.header_hostname_.compare(handler_ptr->http_request_.uri_hostname_))
		{
			set_error_response(	handler_ptr, \
									"URI hostname doesn't match HEADER-HOST hostname: " + \
									handler_ptr->http_request_.uri_hostname_ + \
									" vs " + \
									handler_ptr->http_request_.header_hostname_ + \
									ft_code_location(__FILE__, __LINE__), \
								400);
			switch_to_prepare_response(handler_ptr);
			return (-1);
		}
	}
	handler_ptr->http_request_.header_hostname_ = header_hostname;
	
	if (header_port_str.empty() || header_port_str.size() > 5 || header_port_str.find_first_not_of(DIGIT_RADIX_) != std::string::npos)
	{
		set_error_response(handler_ptr, "Header port is invalid: " + header_port_str + ft_code_location(__FILE__, __LINE__), 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}

	handler_ptr->http_request_.header_port_ = std::atoi(header_port_str.c_str());
	if (handler_ptr->http_request_.header_port_ && handler_ptr->http_request_.header_port_ != (handler_ptr->host_port_ << 32 >> 32))
	{
		set_error_response(handler_ptr, "Header port is invalid: " + header_port_str + ft_code_location(__FILE__, __LINE__), 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}	
		
	return (0);
}
