/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_08_parse_request_line.cpp              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:27:08 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>

//	everage request size is between 800B and 2KB
#define BUFFER_SIZE 4096

//	HTTP/1.0 or HTTP/1.1 : 8 charachters
#define HTTP_VERSION_STR_LEN_ 8

//	Max length of method name: 'delete' is the longest one with length 6
#define METHOD_MAX_LEN_ 6

//	ToDo : DO NOT SET ERROR RESPONSE INSIDE, we need it to parse CGI output
//	RETURN:  
//			 1	not yet recieved request line completely
//			 0	already parsed request line
//			 0	parsed SUCCESS
//			-1	bad request (invalid syntax, unsupported method)
int		AppRun::parse_request_line(Handler* handler_ptr, std::string& request_str)
{
	HTTP_Request& request = handler_ptr->http_request_;

	if (request.is_parsed_request_line_ == YES_)
		return (0);
	
	//	HTTP 1.0 : https://www.rfc-editor.org/rfc/rfc1945#section-4
	//	HTTP 1.1 : https://www.rfc-editor.org/rfc/rfc9112#name-request-line
	//
	//	Request-Line = Method SP Request-URI SP HTTP-Version CRLF
	//
		
	std::string::size_type crlf_pos = request_str.find(RCLF_);
	std::string::size_type crlf_crlf_pos = request_str.find(RCLF_RCLF_);

	//	In the interest of robustness, a server that is expecting to receive and parse a request-line SHOULD ignore at least one empty line (CRLF) received prior to the request-line
	//	https://www.rfc-editor.org/rfc/rfc9112#name-message-format
	//	here we find next RCLF, if the first one is ignored
	while (!crlf_pos)
	{
		request_str.erase(0, 2);
		crlf_pos = request_str.find(RCLF_);
	}
	
	//	quit if NOT received request-line COMPLETELY
	//	otherwise set flag=YES_ and parse it
	if (crlf_pos == std::string::npos)
	{
		ft_code_location(__FILE__, __LINE__);
		return (1);
	}
	else
		request.is_parsed_request_line_ = YES_;


	//	When a server listening only for HTTP request messages, or processing what appears from the start-line to be an HTTP request message, 
	//	receives a sequence of octets that does not match the HTTP-message grammar aside from the robustness exceptions listed above, 
	//	the server SHOULD respond with a 400 (Bad Request) response and close the connection.
	//	https://www.rfc-editor.org/rfc/rfc9112#name-message-format
	//
	//	400 : BAD REQUEST
	

	std::string request_line = request_str.substr(0, crlf_pos);

	// here we erase request line, including 'leading CRLF' and '2 characters of CRLF' or '4 chars CRLF_CRLF_' if 'request == request_line CRLF_CRLF'
	int crlf_len = 2 + (crlf_pos == crlf_crlf_pos) * 2;
	request_str.erase(0, crlf_pos + crlf_len);


	//	HTTP 1.1 not recommends multi-SP (SP = space) and other whitespaces in addition to SP (HTUB, VT, FF)
	//	we do not allow HTUB, VT, FF and also do not allow consecutive SP and leading SP
	//	we do not allow trailing space: <requst>CRLF<space>
	//	should be only 2 SP
	//	fixed length = 8 segment after 2nd SP: HTTP/1.0 or HTTP/1.1
	std::string::size_type wrong_delimiter = request_line.find_first_of("\t\v\f\r");
	std::string::size_type wrong_multispace = request_line.find("  ");
	std::string::size_type space_1_pos = request_line.find(' ');
	std::string::size_type space_2_pos = request_line.find(' ', space_1_pos + 1);
	std::string::size_type space_3_pos = request_line.find(' ', space_2_pos + 1);

	//	400 : BAD REQUEST
	if (wrong_delimiter != std::string::npos || \
		wrong_multispace != std::string::npos || \
		space_1_pos == 0 || \
		space_1_pos == std::string::npos || \
		space_2_pos == std::string::npos || \
		space_3_pos != std::string::npos || \
		(request_line.size() - space_2_pos - 1) != HTTP_VERSION_STR_LEN_)
	{
		set_error_response(handler_ptr, request_line, 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);		
	}
	
	std::string method_str = request_line.substr(0, space_1_pos);
	//	capitalize method
	//	the bellow will not work untill c++11
	//		https://stackoverflow.com/questions/21805674/do-i-need-to-cast-to-unsigned-char-before-calling-toupper-tolower-et-al
	// std::transform(method_str.begin(), method_str.end(), method_str.begin(), [](unsigned char c){return std::toupper(c);});
	std::transform(method_str.begin(), method_str.end(), method_str.begin(), ft_toupper);
	
	
	handler_ptr->http_request_.method_ =	DELETE_ * (method_str == "DELETE") + \
											GET_ * (method_str == "GET") + \
											POST_ * (method_str == "POST");
	handler_ptr->http_request_.method_str_ = method_str;

		
	//	we support only 3 methods, if anything else is requested - error response
	if (!handler_ptr->http_request_.method_)
	{
		set_error_response(handler_ptr, request_line, 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}
	
	//	no check of URI at this stage
	//	IMPORTANT: URI is CASE-SENSETIVE ! though some part of it are NOT case-sensetive
	handler_ptr->http_request_.uri_ = request_line.substr(space_1_pos + 1, (space_2_pos - space_1_pos - 1));

	//	400 : BAD REQUEST
	//	we support only HTTP/1.0 and HTTP/1.1
	std::string http_version_str = request_line.substr(space_2_pos + 1, HTTP_VERSION_STR_LEN_);
	if (http_version_str.find("HTTP/1.") != 0 || \
		http_version_str.size() != HTTP_VERSION_STR_LEN_ || \
		(http_version_str[HTTP_VERSION_STR_LEN_ - 1] != '0' && http_version_str[HTTP_VERSION_STR_LEN_ - 1] != '1'))
	{
		set_error_response(handler_ptr, request_line, 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}
	else
		handler_ptr->http_request_.http_version_ = (http_version_str[HTTP_VERSION_STR_LEN_ - 1] == '1');


	return (0);
}

// SHOW ME AT LEAST ONE GOOD REASON TO USE c++98 ????!!!!
// (http_version_str.back() != '0' && http_version_str.back() != '1')

