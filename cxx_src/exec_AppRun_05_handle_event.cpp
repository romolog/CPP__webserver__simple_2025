/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_05_handle_event.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:03:09 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_SignalHandler.hpp"
#include "all_utils.hpp"

#include <iostream>

void	AppRun::handle_epoll_events(void)
{
	if (SignalHandler::signum_)
		return ;
		
	if (epoll_count_ == -1)
		throw AppException(prog_name_, "System error: Can't get epoll_wait: 23");

	for(int i = 0; i < epoll_count_; ++i)
	{
		uint32_t event_type = epoll_events_ptr_[i].events;
		int handler = epoll_events_ptr_[i].data.u64 >> 32;
		int socket_fd = static_cast<int>(epoll_events_ptr_[i].data.u64 << 32 >> 32);

		//	handler = type of socket, either LISTEN or 3in1 (RECV_SEND_CLOSE)
		//	handling_type = what type of handling based on event and type of socket
		//		example:
		//			event happend for listening socket
		//			handler = LISTEN_ = socket, which is listening for connection request
		//			event = EPOLLIN = socket received connection request
		//			handling_type = ACCEPT_F_ = we will call accept() function to process connection request
		
		unsigned handling_case = 	ACCEPT_F_ *	(!(event_type & EPOLLHUP) && (event_type & EPOLLIN)		&& (handler == HANDLER_LISTEN_)) + \
									RECV_F_ *	(!(event_type & EPOLLHUP) && (event_type & EPOLLIN)		&& (handler == HANDLER_RECV_)) + \
									SEND_F_ *	(!(event_type & EPOLLHUP) && (event_type & EPOLLOUT)	&& (handler == HANDLER_SEND_)) + \
									CLOSE_F_ *	( (event_type & EPOLLHUP) != 0);


		switch (handling_case)
		{
			case ACCEPT_F_:
			{
				accept_connection(socket_fd);
				break;
			}
			case RECV_F_:
			{
				int check_read = read_request(socket_fd);
				if (check_read <= 0)
					close_connection(socket_fd);
				break;
			}
			case SEND_F_:
			{
				int check_responce = send_response(socket_fd);
				if (check_responce < 0)
					close_connection(socket_fd);
				break;
			}
			case CLOSE_F_:
			{
				close_connection(socket_fd);
				break;
			}
			default:
			{
				std::cerr	<< TAB_ << prog_name_ << YELLOW_BOLD_HIGH_ << WARNING_ << DEFAULT_COLOR_ \
							<< "Unsupported event happened and ignored: Event epoll mask is: " << event_type << "Socket type is: " << handler << '\n';
				break;
			}
		}
	}
}
