/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_17_close.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 20:28:30 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <iostream>


//	A server that receives an incomplete request message, 
//	usually due to a canceled request or a triggered timeout exception, 
//	MAY send an error response prior to closing the connection.

void	AppRun::close_connection(const int client_socket_fd)
{
	//	TO CLOSE CONNECTION:
	//		- delete from epoll interest list
	//		- delete from app_handlers<>_
	//		- close socket_fd and set it -1: this will be double checked by Dtor of HTTP_Client

	delete_from_epoll(client_socket_fd);

	delete_handler_from_map(client_socket_fd, app_handlers_can_read_);
	delete_handler_from_map(client_socket_fd, app_handlers_prepare_response_);
	delete_handler_from_map(client_socket_fd, app_handlers_can_response_);
	delete_handler_from_map(client_socket_fd, app_handlers_);	

	// delete (app_handlers_[client_socket_fd]);
	// app_handlers_[client_socket_fd] = NULL;

	if (client_socket_fd > 2)
	{
		int check_close = close(client_socket_fd);
		#ifndef NDEBUG
		if (check_close == -1)
			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_  << DEFAULT_COLOR_ <<"Closing connection: Error: Can't close socket fd: " << client_socket_fd << '\n';
		#else
			(void)check_close;
		#endif
		return ;
	}
	
	if (client_socket_fd >= 0 && client_socket_fd <= 2)
		throw AppException(prog_name_, "Algorithm error" + ft_code_location(__FILE__, __LINE__));
}

