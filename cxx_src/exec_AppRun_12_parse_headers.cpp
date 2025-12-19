/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_12_parse_headers.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 20:16:24 by rponomar         ###   ########.fr       */
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

//		-	parse HEADERS
//				field-line   = field-name ":" OWS field-value OWS
//				https://www.rfc-editor.org/rfc/rfc9112#name-field-syntax
//				Each field line consists of a case-insensitive field name followed by a colon (":"), optional leading whitespace, the field line value, and optional trailing whitespace.
//					- on invalid input switch to ERROR-RESPONSE: 400 BAD REQUEST
//					- otherwise save HEADERS to hash-table

//	\r\n\r\n - is used ALWAYS to delimite headers from the body (first \r\n is end of the last header field, the second \r\n is a delimiter empty line)
//	\r\n\r\n - is used OFTEN to define that final chunk of request is sent (NODE.JS uses  '0\r\n\r\n')


//	CHUNK EXAMPLE:
//		HTTP/1.1 200 OK
//		Transfer-Encoding: chunked
//		Content-Type: text/html
//
//		C\r\n
//		h1>go!</h1>\r\n
//
//		1B\r\n
//		<h1>first chunk loaded</h1>\r\n
//
//		2a\r\n
//		<h1>second chunk loaded and displayed</h1>\r\n
//
//		29\r\n
//		<h1>third chunk loaded and displayed</h1>\r\n
//
//		0\r\n\r\n
//
//		Trailing_field: trail_field_value\r\n
//		\r\n
//	
//	END OF EXAMPLE

//	MORE ABOUT CHUNKED:
//	https://stackoverflow.com/questions/5590791/http-chunked-encoding-need-an-example-of-trailer-mentioned-in-spec
//	http://www.tcpipguide.com/free/t_HTTPDataLengthIssuesChunkedTransfersandMessageTrai-3.htm


//	RETURN:  
//			 1 : headers are not received completely
//			 0 : already parsed headers
//			 0 : parsed SUCCESS
//			-1 : bad request (invalid syntax, unsupported method)
int		AppRun::parse_headers(const int , Handler* handler_ptr, std::string& request_str)
{
	HTTP_Request& request = handler_ptr->http_request_;
	
	if (request.is_parsed_request_line_ == NO_ || request.is_parsed_headers_ == YES_)
		return (request.is_parsed_headers_ == NO_);

	//	the stderr log error_message (do not confuse with error_response to the client) will be set inside 'parse_headers_trailers()'
	std::string err_msg;

	std::string::size_type crlf_crlf_pos = request_str.find(RCLF_RCLF_);
	if (crlf_crlf_pos == std::string::npos)
	{
		return (1);
	}
	else
		request.is_parsed_headers_ = YES_;

	int check_headers_trailers = read_headers_trailers(request_str, handler_ptr, err_msg, handler_ptr->http_request_.headers_, RCLF_);
	if (check_headers_trailers == -1)
	{
		set_error_response(handler_ptr, err_msg + ft_code_location(__FILE__, __LINE__), 400);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}

	//	check 'Transfer-encoding' header:
	//		SUCCESS : no such header
	//		SUCCESS : there is the header, and the only field_body = "chunked"
	//		ERROR	: any other case
	//	set CHUNKED flag to 1, if there is "Transfer-encoding: Chunked"
	if (handler_ptr->http_request_.headers_.count("TRANSFER-ENCODING"))
	{
		std::string transfer_encoding_str = handler_ptr->http_request_.headers_["TRANSFER-ENCODING"].second;
		std::transform(transfer_encoding_str.begin(), transfer_encoding_str.end(), transfer_encoding_str.begin(), ft_toupper);
		if (transfer_encoding_str != "CHUNKED")
		{
			set_error_response(handler_ptr, "Invalid 'Transfer-Encoding' header" + ft_code_location(__FILE__, __LINE__), 400);
			switch_to_prepare_response(handler_ptr);
			return (-1);
		}
		handler_ptr->http_request_.is_chunked_ = YES_;
	}


	if (handler_ptr->http_request_.headers_.count("CONTENT-LENGTH"))
	{
		//	https://www.rfc-editor.org/rfc/rfc9112#name-content-length
		//	when both Content-Length and Transfer-Encoding headers are present, then it is an error
		if (handler_ptr->http_request_.is_chunked_)
		{
			set_error_response(handler_ptr, "Invalid concurrence of 'Transfer-Encoding' and 'Content-Length' headers" + ft_code_location(__FILE__, __LINE__), 400);
			return (-1);
		}

		std::string content_length_str = handler_ptr->http_request_.headers_["CONTENT-LENGTH"].second;
		std::string::size_type	wrong_char = content_length_str.find_first_not_of(DIGIT_RADIX_);
		if (content_length_str.empty() || wrong_char != std::string::npos)
		{
			set_error_response(handler_ptr, "Invalid 'Content-Length' header" + ft_code_location(__FILE__, __LINE__), 400);
			return (-1);
		}
		handler_ptr->http_request_.content_length_ = std::atoi(content_length_str.c_str());
	}


	if (request.headers_["HOST"].first != 1 ||
		request.headers_["HOST"].second.empty())
	{
		set_error_response(handler_ptr, "Invalid 'Host': Missing or more than one" + ft_code_location(__FILE__, __LINE__), 400);
		return (-1);
	}
	
	handler_ptr->http_request_.is_trailers_ = handler_ptr->http_request_.headers_.count("TRAILER");

	return (0);
}
