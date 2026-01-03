/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_01_main_loop.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 19:58:04 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_SignalHandler.hpp"
#include "all_utils.hpp"

#include <climits>
#include <iostream>

//	IMPORTANT: read it !!!
//		https://www.rfc-editor.org/rfc/rfc9112#name-associating-a-response-to-a


//	Start EPOLL()
//	LOOP:
//		update and get EPOLL() events
//		handle EVENTS:
//			ACCEPT
//			RECIEVE
//			SEND
//			CLOSE
//		kill TIMEOUT
//		PREPARE response
//	End EPOLL()
void	AppRun::main_loop(void)
{
	//	create epoll instance
	start_epoll();

	//	add listeners(http_servers) to epoll interest list
	populate_epoll_with_listeners();

	//	calculate initial max num of epoll events
	//	in general mux_num_of_events = (num_of_servers + num_of_clients)
	listeners_num_ = app_listeners_.size();
	epoll_max_num_ = listeners_num_;

	// only signals can make do_exit_ = 1;
	while(!SignalHandler::signum_)
	{
		
		update_epoll_buffer();
		
		//	--------------------EPOLL WAIT----------------------------------------------------------
		epoll_count_ = epoll_wait(epoll_fd_, epoll_events_ptr_, epoll_max_num_, EPOLL_WAIT_TIMEOUT_);

		
		//	depending on event:
		//		http_server accepts connection request and create a new http_client, adding it to epoll 'interest list'
		//		http_client:
		//				if flag is set to "ready to read" reads request and sets flag for "request received"
		//				if flag is set to "ready to responce" sends responce and sets flag for "ready to read"
		//				if flag is "request recieved" do nothing and waits, when responce is ready
		//				flags could be also set to "reading in progress" for chunked requests
		//				flag could also be set to "responce in progress" for chunked responce
		handle_epoll_events();

		
		//	kill any process which is running more than allowed duration time
		//	kill infinite loops in CGI, etc
		//	try to send error message to client: 408 Timeout request
		//	close connection, if error message is also a timeout
		kill_timeout_processes_handlers();
				
		prepare_response();
	}

	close_all_connections();
	(void) close(epoll_fd_);
}
