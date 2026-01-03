/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_11_parse_uri.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 20:17:31 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"
#include "all_utils.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>

// URI STRUCTURE:
// https://en.wikipedia.org/wiki/Uniform_Resource_Identifier
//
//	HTTP : // userinfo @ host : port /PATH ? query_str # fragment

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
int		AppRun::parse_uri(Handler* handler_ptr, const std::string& uri)
{
	HTTP_Request& request = handler_ptr->http_request_;

	if (request.is_parsed_headers_ == NO_)
		return (1);
	else if (request.is_parsed_uri_ == YES_)
		return (0);
	else
		request.is_parsed_uri_ = YES_;

	if (uri.size() > URI_MAX_LEN_)
	{
		set_error_response(handler_ptr, "Uri is invalid: Too large: " + uri.substr(0, ERROR_INPUT_SNIPET_LEN_), 414);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}

	
	//	"GET	http://127.0.0.1:2442/ 									HTTP/1.1"
	//			http://www.example.org/pub/WWW/TheProject.html 			
	//			http://foo:bar@example.com:8080/search?q=word#results
	//			/dir1/dir2/file?a=b&c=d#fragment

	//	GET /pub/WWW/TheProject.html HTTP/1.1
	//	Host: www.example.org:8080
	
	//	to find a start position of PATH in the URI - we need to distinguish between ABSOLUTE and RELATIVE kinds of URI
	//		- ABSOLUTE: 
	//			http://www.example.org/pub/WWW/TheProject.html
	//			path = "/pub/WWW/TheProject.html"
	//			path_pos = 22 = uri.find ('/', after the "http://")
	//		- RELATIVE:
	//			 /dir1/dir2/file?a=b&c=d#fragment
	//			path = /dir1/dir2/file
	//			path_pos = 0

	std::string::size_type scheme_end_pos = uri.find("://");
	std::string scheme = uri.substr(0, scheme_end_pos);

	std::transform(scheme.begin(), scheme.end(), scheme.begin(), ft_tolower);
	
	int is_abs_uri = (scheme == "http" || scheme == "https");

	std::string::size_type path_pos = uri.find('/', is_abs_uri * (scheme_end_pos + 3));
	if ((scheme_end_pos != std::string::npos && !is_abs_uri) || path_pos == std::string::npos)
	{
		set_error_response(handler_ptr, "Uri is invalid: " + uri.substr(0, ERROR_INPUT_SNIPET_LEN_) + ft_code_location(__FILE__, __LINE__), 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}

	std::string::size_type query_pos = uri.find('?', path_pos + 1);
	
	// got URI_PATH
	std::string::size_type uri_len = query_pos - path_pos;
	std::string uri_path = uri.substr(path_pos, uri_len);


	// set original request URI_PATH (will NOT be overidden by CGI_REWRITE)
	handler_ptr->http_request_.uri_path_ = uri_path;

	// got URI_EXT
	std::string::size_type dot_pos = uri_path.find('.');

	std::string::size_type path_info_pos = uri_path.find(dot_pos, '/');	

	std::string uri_path_ext = "";
	if (dot_pos != std::string::npos)
		uri_path_ext = uri_path.substr(dot_pos, path_info_pos - dot_pos);
		
	// set URI_EXT, it remains even if we apply CGI_REWRITE
	handler_ptr->http_request_.uri_ext_ = uri_path_ext;

	//	got CGI_EXT from 'Server' level
	std::string cgi_server_ext = handler_ptr->config_server_.cgi_ext_;

	//	got CGI STATUS at 'Server' level : it may be changed at 'Location' level in 'assign_location_to_request()'
	if (uri_path_ext.empty() || cgi_server_ext.empty())
		handler_ptr->http_request_.is_cgi_ = NO_;
	else if (uri_path_ext == cgi_server_ext)
		handler_ptr->http_request_.is_cgi_ = YES_;

	//	got URI_PATH_INFO 
	//		only CGI can have PATH_INFO, but maybe the request becomes CGI only at 'Location' level
	//		IMPORTANT: we do NOT rewrite path-info, even if CGI_REWRITE is applicable
	std::string uri_path_info = "";
	if (path_info_pos != std::string::npos)
		uri_path_info = uri_path.substr(path_info_pos);

	// set URI_PATH_INFO
	handler_ptr->http_request_.uri_path_info_ = uri_path_info;

	// got URI_TARGET, which maybe rewritten by CGI_REWRITE bellow
	std::string uri_target = uri_path.substr(0, path_info_pos);

	// apply CGI_REWRITE if applicable
	// got FINAL URI_TARGET
	std::string cgi_server_rewrite = handler_ptr->config_server_.cgi_rewrite_;
	if (handler_ptr->http_request_.is_cgi_ == YES_ && !cgi_server_rewrite.empty())
		uri_target = cgi_server_rewrite;

	// can check only 'target' without PATH_INFO
	std::string err_msg;
	if (!ft_is_valid_pathname(uri_target, err_msg))
	{
		set_error_response(handler_ptr, err_msg, 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);		
	}

	//	set FINAL URI TARGET
	handler_ptr->http_request_.uri_target_ = uri_target;


	if (is_abs_uri)
	{
		// get URI_HOSTNAME
		std::string::size_type ed_pos = uri.find('@');
		std::string::size_type start_pos = (ed_pos != std::string::npos ? (ed_pos + 1) : (scheme_end_pos + 3));
		std::string::size_type colon_pos = uri.find(start_pos, ':');
		std::string::size_type end_pos = (colon_pos != std::string::npos ? colon_pos : path_pos);				
		std::string uri_hostname = uri.substr(start_pos, end_pos - start_pos);

		std::string err_msg;
		int check_hostname = ft_is_valid_hostname(uri_hostname, err_msg);
		if (check_hostname)
		{
			set_error_response(handler_ptr, err_msg, 400);
			switch_to_prepare_response(handler_ptr);
			return (-1);
		}
		std::transform(uri_hostname.begin(), uri_hostname.end(), uri_hostname.begin(), ft_tolower);

		// set URI_HOSTNAME
		handler_ptr->http_request_.uri_hostname_ = uri_hostname;
	}

	std::string::size_type fragment_pos = uri.find('#', query_pos + 1);
	//	get URI QUERY STRING
	//		ignore query for any method except GET
	//		query is part of uri, following the first '?' and before '#'
	std::string query_str = "";
	if (handler_ptr->http_request_.method_str_ == "GET" && query_pos != std::string::npos)
		query_str = uri.substr(query_pos, fragment_pos - query_pos);

	//	set URI QUERY STR
	handler_ptr->http_request_.uri_query_str_ = query_str;

	return (0);
}

