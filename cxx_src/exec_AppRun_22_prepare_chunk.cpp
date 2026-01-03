/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_22_prepare_chunk.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:27:39 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>


	// CHUNK EXAMPLE:
	// HTTP/1.1 200 OK
	// Content-Type: text/plain
	// Transfer-Encoding: chunked
	// \r\n
	// 7\r\n
	// Mozilla\r\n
	// 11\r\n
	// Developer Network\r\n
	// 0\r\n
	// \r\n

void	AppRun::prepare_chunk(Handler *handler_ptr)
{
	HTTP_Response& response = handler_ptr->http_response_;	

	std::string& body = response.full_body_;
	std::string::size_type body_size = body.size();
	std::string::size_type chunk_size = SERVER_BUFFER_SIZE_ - 256;
	if (chunk_size > body_size)
		chunk_size = body_size;

	response.response_str_ += ft_itoa(chunk_size) + RCLF_ + body.substr(0, chunk_size);

	body.erase(0, chunk_size);

	if (!chunk_size)
	{
		response.is_final_chunk_ = YES_;
	}

	handler_ptr->handler_status_ = READY_TO_RESPONSE_;
}

