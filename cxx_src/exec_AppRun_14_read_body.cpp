/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_14_read_body.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:15:24 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "wbs_defaults.hpp"
#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

//	everage request size is between 800B and 2KB
#define BUFFER_SIZE 4096

//	HTTP/1.0 or HTTP/1.1 : 8 charachters
#define HTTP_VERSION_STR_LEN_ 8

//	Max length of method name: 'delete' is the longest one with length 6
#define METHOD_MAX_LEN_ 6

//	WHAT IS BODY:
//		SECTION 6.4.2 Identifying Content
//			not helpful for parsing :((((


//	PARSE BODY:
//		- is ther anybody outthere: YES if any char presents after empty line
//		- should there be anbody outthere: YES - if either Content-Length or Encoding-Type
//		- how to check the body delimiter = content-length and header Trailers
//		- chunked or not
//		- if CHUNKED:
//			- get size
//			- read chunk
//			- check if read = size, otherwise return set_error_response: BAD REQUEST
//		- if NOT chunked:
//			- get

//	https://www.rfc-editor.org/rfc/rfc9112#name-field-syntax
//	A server MAY reject a request that contains a message body but not a Content-Length by responding with 411 (Length Required)


// 7.1.3. Decoding Chunked
//		https://www.rfc-editor.org/rfc/rfc9112#name-chunked-transfer-coding

//	RETURN:  
//			 1 : not ready to parse
//			 0 : already parsed
//			 0 : success
//			-1 : error
//			 0 : do NOT read body for DETELE and GET
int		AppRun::parse_body(Handler* handler_ptr, std::string& request_str)
{
	
	if (request_str.find_first_not_of("\r\f\v\t\n ") == std::string::npos)
		request_str = "";


	HTTP_Request& request = handler_ptr->http_request_;

	if (request.is_parsed_headers_ == NO_)
		return (1);
	else if (request.is_parsed_body_ == YES_)
		return (0);


	//	for method DELETE do not read body
	if (handler_ptr->http_request_.method_ == DELETE_)
	{
		request.is_parsed_body_ = YES_;
		return (0);
	}

	if (request.max_body_ == NOT_SET_)
	{

		int max_body_size_location = handler_ptr->config_location_.client_max_body_size_;
		int max_body_size_server = handler_ptr->config_server_.client_max_body_size_;
		int max_body_size = (max_body_size_location != NOT_SET_) ? max_body_size_location : max_body_size_server;
		max_body_size = (max_body_size == NOT_SET_) ? REQUEST_MAX_BODY_SIZE_ : max_body_size;
		request.max_body_ = max_body_size;
	}


	int is_trailers = request.is_trailers_;
	int is_chunked = request.is_chunked_;
	int body_trailers_size = request_str.size();

	//  ------ PART-1 :  BODY IS NOT CHUNKED ----- 
	if (!is_chunked)
	{		
		// non-negative integer number of octets (maybe 0)
		int content_len = request.content_length_;
		
		if (content_len != NOT_SET_ && content_len > body_trailers_size)
		{
			return (1); // not yet received the body completely OR no body
		}
		else if (content_len == NOT_SET_ && request_str.empty())
		{
			request.is_parsed_body_ = YES_;
			return (0);
		}			
		else if (request.max_body_ != NOT_SET_ && content_len > request.max_body_)
		{
			// 413 Payload Too Large
			set_error_response(handler_ptr, "Client_max_body_size exceeded" + ft_code_location(__FILE__, __LINE__), 413);
			switch_to_prepare_response(handler_ptr);
			return (-1);
		}
		else if (content_len != NOT_SET_ && content_len <= body_trailers_size) // BODY IS RECIEVED COMPLETELY
		{

			request.is_parsed_body_ = YES_;
			request.body_ = request_str.substr(0, content_len);
			request.body_size_ = content_len;
			request_str.erase(content_len);
			return (0);
		}	
		else if (content_len == NOT_SET_ && !is_trailers && !request_str.empty())
		{
			set_error_response(handler_ptr, "Body presents, length is required" + ft_code_location(__FILE__, __LINE__), 411);
			switch_to_prepare_response(handler_ptr);
			return (-1);
		}
		else if (content_len != NOT_SET_ && request_str.empty())
		{
			set_error_response(handler_ptr, "Body missing" + ft_code_location(__FILE__, __LINE__), 400);
			switch_to_prepare_response(handler_ptr);
			return (-1);
		}		
		else if (content_len == NOT_SET_ && is_trailers)
		{
			// no body, only trailers
			request.is_parsed_body_ = YES_;
			return (0);
		}

		set_error_response(handler_ptr, "Algorithm error" + ft_code_location(__FILE__, __LINE__), 500);
		switch_to_prepare_response(handler_ptr);
		return (-1);
	}
	else // ------------- PART-2 : BODY IS CHUNKED ---------
	{
		
		while (request.is_parsed_body_ == NO_ && !request_str.empty())
		{		
			
			std::string::size_type rclf_1_pos = request_str.find(RCLF_);
			while(!rclf_1_pos)
			{
				request_str.erase(0, 2);
				rclf_1_pos = request_str.find(RCLF_);
			}

			if (rclf_1_pos == std::string::npos) // only fragment remains, not a full chunk
				return (1);
				
			std::string chunk_size_str = request_str.substr(0, rclf_1_pos);
			std::string::size_type wrong_char = chunk_size_str.find_first_not_of(HEX_RADIX_);
			if (wrong_char != std::string::npos)
			{
				set_error_response(handler_ptr, "Chunk is invalid: " + request_str.substr(0, ERROR_INPUT_SNIPET_LEN_) + ft_code_location(__FILE__, __LINE__), 400);
				switch_to_prepare_response(handler_ptr);
				return (-1);
			}

			std::stringstream ss;
			ss << std::hex << chunk_size_str;
			int chunk_size;
			ss >> chunk_size;

			std::string::size_type rclf_2_pos = request_str.find(RCLF_, rclf_1_pos + 2);
			int	chunk_received_size = rclf_2_pos - rclf_1_pos - 2;

			if (rclf_2_pos == std::string::npos || chunk_size > chunk_received_size)
			{
				// only fragment remains, not a full chunk
				return (1);
			} 
			else if (chunk_size < chunk_received_size)
			{				
				set_error_response(handler_ptr, "Chunk is invalid: " + request_str.substr(0, ERROR_INPUT_SNIPET_LEN_) + ft_code_location(__FILE__, __LINE__), 400);
				switch_to_prepare_response(handler_ptr);
				return (-1);
			}
			else if (request.body_size_ + chunk_size > request.max_body_)
			{
				// 413 Payload Too Large
				set_error_response(handler_ptr, "Client_max_body_size exceeded" + ft_code_location(__FILE__, __LINE__), 413);
				switch_to_prepare_response(handler_ptr);
				return (-1);
			}
			else if (chunk_size == chunk_received_size)
			{

				request.is_all_chunks_recieved_ = (chunk_size == 0);
				request.is_parsed_body_ = (chunk_size == 0);
				request.body_ += request_str.substr(rclf_1_pos + 2, chunk_size);
				request.body_size_ += chunk_size;
				request_str.erase(0, rclf_2_pos + 2);
				if (chunk_size)
					continue;
				else
					return (1);
			}

			
			set_error_response(handler_ptr, "Algorithm error" + ft_code_location(__FILE__, __LINE__), 500);
			switch_to_prepare_response(handler_ptr);
			return (-1);
		}

		if (request.is_parsed_body_ == NO_ && request_str.empty())
		{
			return (1);
		}
	}
	
	return (0);
}

