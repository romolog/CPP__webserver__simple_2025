/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_51_assemlbe_chunk_or_unchunk.cpp       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 17:14:57 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>

//	SET based on body.empty and (body_size < SERVER_BUFFER_SIZE_ - 256):
//		EMPTY BODY:
//			response.response_str_ = status_line + RCLF_;	
//		CHUNKED: 
//			response.is_chunked_ = YES_
//			response.response_str_ = status_line + content_type + transfer_encoded + RCLF_;
//		UNCHUNKED:
//			response.response_str_ = status_line + content_type + content_len + RCLF_ + body;
void	AppRun::assemble_chunk_or_unchunk(Handler* handler_ptr, HTTP_Response& response, const std::string& status_line, const std::string& content_type)
{
	std::string& body = response.full_body_;
	long long body_size = body.size();


	if (body.empty())
		response.response_str_ = status_line + RCLF_;
	else if (body_size < SERVER_BUFFER_SIZE_ - 256)
	{
		std::string content_len = "CONTENT-LENGTH: " + ft_itoa(body_size) + RCLF_;
		response.response_str_ = status_line + content_type + content_len + RCLF_ + body;
	}
	else
	{

		response.is_chunked_ = YES_;
		std::string transfer_encoded = "TRANSFER-ENCODING: CHUNKED\r\n";
		response.response_str_ = status_line + content_type + transfer_encoded + RCLF_;

		prepare_chunk(handler_ptr);
	}
}

