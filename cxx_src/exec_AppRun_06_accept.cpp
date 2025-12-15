/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_06_accept.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:03:31 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <fcntl.h>

void	AppRun::accept_connection(const int listener_socket_fd)
{	
	Listener* listener_ptr = app_listeners_[listener_socket_fd];
	ServerConfig& config_server = app_config_[listener_ptr->host_port_];

	//	create a new Handler
	Handler* new_handler_ptr = new Handler(prog_name_, config_server);
	new_handler_ptr->host_port_ = listener_ptr->host_port_;


	sockaddr* client_sockaddr_ptr = reinterpret_cast<sockaddr*>(&(new_handler_ptr->sockaddr_in_));
	socklen_t client_sockaddr_len = 0;

	//	accept() can't set O_NONBLOCK flag, only GNU accept4() can
	//	so we need to call fcntl() for nonblocking
	new_handler_ptr->socket_fd_ = accept(listener_socket_fd, client_sockaddr_ptr, &client_sockaddr_len);


	if (new_handler_ptr->socket_fd_ == -1)
	{
		delete new_handler_ptr;

		std::stringstream line_num_ss;
		line_num_ss << __LINE__;
		throw AppException(prog_name_, "System error: Can't open SOCK_STREAM socket fd under protocol IPPROTO_TCP", __FILE__, line_num_ss.str());
	}

	//	SUBJECT REQUEST FOR NON-BLOCKING SOCKETS:
	//	Because you have to use non-blocking file descriptors ... 
	int check_fcntl = fcntl(new_handler_ptr->socket_fd_, F_SETFL, O_NONBLOCK | O_CLOEXEC);
		if (check_fcntl == -1)
			std::cout << TAB_ << prog_name_ << YELLOW_BOLD_HIGH_ << WARNING_ << DEFAULT_COLOR_ << "System error: Can't fcntl O_NONBLOCK\n";
		
	//	set its status to "READY_TO_READ_" : if it recieves a request, it will read it (other status prevents from reading new request before the previous is answered)
	new_handler_ptr->handler_status_ = READY_TO_READ_;

	//	add new client to general list
	app_handlers_.insert(std::pair<int, Handler*>(new_handler_ptr->socket_fd_, new_handler_ptr));

	//	add new client to 'ready to read' list - 
	app_handlers_can_read_.insert(std::pair<int, Handler*>(new_handler_ptr->socket_fd_, new_handler_ptr));

	add_to_epoll(new_handler_ptr->socket_fd_, HANDLER_RECV_, (EPOLLIN | EPOLLHUP));

	//	set timeout: if during timout duration at least one chunk of request is not recieved, we close this client and connection socket
	new_handler_ptr->set_timeout(TIMEOUT_FOR_GETTING_REQUEST_);

	// IMPORTANT: assign REDIRECT from SERVER level
	if (config_server.redirection_code_ != NOT_SET_)
	{
		new_handler_ptr->http_request_.redirection_code_ = config_server.redirection_code_;
		new_handler_ptr->http_request_.redirection_path_ = config_server.redirection_path_;
	}
}
