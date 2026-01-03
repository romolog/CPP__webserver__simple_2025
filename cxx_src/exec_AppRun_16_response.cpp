/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_16_response.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:44:24 by rponomar         ###   ########.fr       */
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


//	RETURN:  
//	1 : on success
//	0 : skip (responce is not yet ready)
//	-1: on error	
int		AppRun::send_response(const int handler_socket_fd)
{
	if (!app_handlers_can_response_.count(handler_socket_fd))
		return (0);
		
	Handler* handler_ptr = app_handlers_can_response_.at(handler_socket_fd);
	HTTP_Response& response = handler_ptr->http_response_;

	// MSG_DONTWAIT : 
	std::size_t response_len = response.response_str_.size();
	ssize_t check_send = send(handler_socket_fd, response.response_str_.c_str(), response_len, MSG_DONTWAIT);
	if (check_send == -1 || (static_cast<std::size_t>(check_send) != response_len))
	{
		return (-1);
	}
	else if (check_send == 0)
	{
		return (-1);
	}
	
	//	close connection after response IF: error-response or HTTP 1.0
	if (response.is_chunked_ == YES_ && response.is_final_chunk_ == NO_)
	{
		delete_from_epoll(handler_socket_fd);
		response.response_str_ = "";
		app_handlers_prepare_response_.insert(std::pair<int, Handler*>(handler_socket_fd, handler_ptr));
	}
	else if (response.error_ == NO_ && handler_ptr->http_request_.http_version_ == 1 &&
		(response.is_chunked_ == NO_ || \
		(response.is_chunked_ == YES_ && response.is_final_chunk_ == YES_)))
	{
		delete_from_epoll(handler_socket_fd);
		handler_ptr->reset();
		add_to_epoll(handler_socket_fd, HANDLER_RECV_, (EPOLLIN | EPOLLHUP));
		handler_ptr->handler_status_ = READY_TO_READ_;
		handler_ptr->set_timeout(TIMEOUT_FOR_GETTING_REQUEST_);
	
		app_handlers_can_read_.insert(std::pair<int, Handler*>(handler_socket_fd, handler_ptr));
	}
	else if (response.error_ == YES_ || handler_ptr->http_request_.http_version_ == 0)
		return (-1);

	delete_handler_from_map(handler_socket_fd, app_handlers_can_response_);
	return (1);
}
