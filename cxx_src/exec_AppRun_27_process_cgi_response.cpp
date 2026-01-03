/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_27_process_cgi_response.cpp            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 19:43:45 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/wait.h>


//	MAIN SOURCE:
//		https://www.rfc-editor.org/rfc/rfc3875#section-6.1
//
//		The response comprises a message-header and a message-body, separated
//		by a blank line.  The message-header contains one or more header
//		fields.  The body may be NULL.

// Status: 500 Internal Server Error
// Content-Type: text/html; charset=utf-8
//
// PATH_INFO not found%    

//	SET:
//		cgi_response.content_type_
//		cgi_response.ref_returned_ : set to ERROR if present
//		cgi_response.status_
//		cgi_response.code_
//		cgi_response.body_
//		cgi_response.content_len_ : based on body.size() + CRLF_CRLF
//		cgi_response.headers_
//	RETURN: 
//		 0 : success
//		-1 : error
int		AppRun::process_cgi_response(Handler *handler_ptr)
{
	HTTP_Request& request = handler_ptr->http_request_;
	HTTP_Response& response = handler_ptr->http_response_;
	CGI_Response& cgi_response = handler_ptr->cgi_response_;

	if (cgi_response.response_str_.empty())
	{
		// 500 Internal Server Error
		set_error_response(handler_ptr, "CGI output is empty: " + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return (-1);
	}

	// parse_headers_trailers	(std::string& input_str, Handler* handler_ptr, std::string& err_msg, t_hash_table_headers& hash_table, const std::string& delimiter);		
	std::string err_msg;
	int check = read_headers_trailers(cgi_response.response_str_, handler_ptr, err_msg, handler_ptr->cgi_response_headers_, "\n");
	if (check == -1)	
	{
		// 500 Internal Server Error
		set_error_response(handler_ptr, err_msg + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return (-1);
	}

	
	cgi_response.body_ = "";
	if (cgi_response.response_str_.find_first_not_of(" \t\r\n\f\v") != std::string::npos)
		cgi_response.body_ = cgi_response.response_str_ + RCLF_RCLF_;


	cgi_response.content_len_ = "CONTENT-LENGTH: " + ft_itoa(cgi_response.body_.size());

	//	check for 3 'CGI-fields'
	//	at lease 1 must have
	//		https://www.rfc-editor.org/rfc/rfc3875#section-4.1.5
	//		The field-name is not case sensitive.  
	//		A NULL field value is equivalent to a field not being sent.
	int is_cgi_status = 0;
	int is_cgi_content_type = 0;
	int is_cgi_location = 0;

	// Content-Type = "Content-Type:" media-type NL
	// 		If an entity body is returned, the script MUST supply a Content-Type
	//		field in the response.  If it fails to do so, the server SHOULD NOT
	//		attempt to determine the correct content type.
	//	https://www.rfc-editor.org/rfc/rfc3875#section-4.1.5
	if (handler_ptr->cgi_response_headers_.count("CONTENT-TYPE"))
	{
		//	if content-type is DUPLICATED or CGI BODY EMPTY, then ERROR
		if (	handler_ptr->cgi_response_headers_["CONTENT-TYPE"].first > 1 || \
				(!handler_ptr->cgi_response_headers_["CONTENT-TYPE"].first && !cgi_response.body_.empty()))
		{
			// 500 Internal Server Error
			set_error_response(handler_ptr, "CGI response error: More than 1 header 'Content-Type' encounted or missing 'Content-Type' with 'Body' present" + ft_code_location(__FILE__, __LINE__), 500);
			prepare_error(handler_ptr);
			return (-1);
		}
		else if (handler_ptr->cgi_response_headers_["CONTENT-TYPE"].first)
		{
			//	if ONLY ONE header content-type is present
			is_cgi_content_type = 1;
			
			std::string  cgi_header_content_type = handler_ptr->cgi_response_headers_["CONTENT-TYPE"].second;

			std::string::size_type content_type_len = cgi_header_content_type.size();
			std::string::size_type lf_pos = cgi_header_content_type.find('\n');
			std::string::size_type cr_pos = cgi_header_content_type.find('\r');

			if (lf_pos != std::string::npos)
			{
				// 500 Internal Server Error
				set_error_response(handler_ptr, "CGI header 'Content-Type' error" + ft_code_location(__FILE__, __LINE__), 500);
				prepare_error(handler_ptr);
				return (-1);
			}
			if (cr_pos == std::string::npos)
				cgi_header_content_type.replace(content_type_len -1, 1, "\r\n");

			cgi_response.content_type_ = "CONTENT-TYPE: " + cgi_header_content_type;
		}
	}

	//		https://www.rfc-editor.org/rfc/rfc3875#section-4.1.5
	//	It is either an absolute
	//	URI (optionally with a fragment identifier), indicating that the
	//	client is to fetch the referenced document, or a local URI path
	//	(optionally with a query string), indicating that the server is to
	//	fetch the referenced document and return it to the client as the response.
	//
	//	WE DO NOT SUPPORT THIS OPTION AND SET ERROR IF FIELD IS PRESENT
	if (handler_ptr->cgi_response_headers_.count("LOCATION"))
	{
		if (handler_ptr->cgi_response_headers_["LOCATION"].first > 1)
		{
			// 500 Internal Server Error
			set_error_response(handler_ptr, "CGI response error: More than 1 header 'Location' encounted" + ft_code_location(__FILE__, __LINE__), 500);
			prepare_error(handler_ptr);
			return (-1);
		}
		else if (handler_ptr->cgi_response_headers_["LOCATION"].first)
		{
			// 501 Not Implemented
			set_error_response(handler_ptr, "CGI response 'Reference instead of requested doc': This option is not supported" + ft_code_location(__FILE__, __LINE__), 501);
			prepare_error(handler_ptr);
			return (-1);
		}
	}

	//	Status	= "Status:" status-code SP reason-phrase NL
	if (handler_ptr->cgi_response_headers_.count("STATUS"))
	{
		// STATUS could be missing, it is 200 OK on default
		std::string cgi_header_status = "200 OK\r\n";
		int code = 200;
		if (handler_ptr->cgi_response_headers_["STATUS"].first > 1)
		{
			// 500 Internal Server Error
			set_error_response(handler_ptr, "CGI response error: More than 1 header 'Status' encounted" + ft_code_location(__FILE__, __LINE__), 500);
			prepare_error(handler_ptr);
			return (-1);
		}
		else if (handler_ptr->cgi_response_headers_["STATUS"].first)
		{
			is_cgi_status = 1;
			cgi_header_status = handler_ptr->cgi_response_headers_["STATUS"].second;
			std::string::size_type status_len = cgi_header_status.size();
			std::string::size_type lf_pos = cgi_header_status.find('\n');
			std::string::size_type cr_pos = cgi_header_status.find('\r');
			std::string code_str = cgi_header_status.substr(0, cgi_header_status.find(' '));
			code = std::atoi(code_str.c_str());
			if (lf_pos != status_len - 1 || \
				cr_pos != std::string::npos || \
				code_str.size() != 3 || \
				code_str.find_first_not_of(DIGIT_RADIX_) != std::string::npos || \
				code < 100 || \
				code > 599)
			{
				// 500 Internal Server Error
				set_error_response(handler_ptr, "CGI header 'STATUS' error: Invalid code: " + code_str + ft_code_location(__FILE__, __LINE__), 500);
				prepare_error(handler_ptr);
				return (-1);
			}
			if (cr_pos == std::string::npos)
				cgi_header_status.replace(status_len -1, 1, "\r\n");
		}
		cgi_response.status_ = cgi_header_status;
		cgi_response.code_ = code;
	}

	// at least 1 CGI field must present
	if (!(is_cgi_content_type + is_cgi_location + is_cgi_status))
	{
		// 500 Internal Server Error
		set_error_response(handler_ptr, "CGI response missing all CGI headers" + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return (-1);
	}

	//	make HTTP-response from CGI-response
	if (cgi_response.status_.empty())
		cgi_response.status_ = " 200 OK\r\n";

	std::string status_line = "HTTP/1." + ft_itoa(request.http_version_) + cgi_response.status_;
	
	//	SET CONTENT TYPE from CGI response, it can NOT be empty if there is non-empty body
	std::string content_type = cgi_response.content_type_;
	if (content_type.empty() && !cgi_response.body_.empty())
	{
		set_error_response(handler_ptr, "CGI response missing all CGI headers" + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return (-1);
	} 
	response.full_body_ = cgi_response.body_;

	//	SET FINAL RESPONSE:
	assemble_chunk_or_unchunk(handler_ptr, response, status_line, content_type);
	handler_ptr->handler_status_ = READY_TO_RESPONSE_;	
	
	return (0);
}