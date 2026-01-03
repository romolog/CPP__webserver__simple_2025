/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_02_add_delete_from_epoll.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 19:58:45 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"

#include <iostream>

void	AppRun::add_to_epoll(const int socket_fd, const int handler_type, uint32_t epoll_type)
{
	epoll_event epoll_info;

	//	epoll_info.data - is a union, it has a field <uint64_t> called 'u64'
	//	we will put combination of socked_fd and handler_type into 'data.u64'
	//		socket_fd - as it is, just casted from uint32 to uint64
	//		handler type - we will cast it first to uint64, then bitwise move left, so it will be larger than any possible socket_fd num
	//			example:	socket_fd = 42 and handler is 'listen' = LISTEN_ = 1
	//						handler will be uint64 1 << 32
	//						their sum will be (0...1...42), alltogether 64 bits: 0...1 is 32bits and 0...42 is 32bits
	uint64_t u64_socket_fd = socket_fd;
	uint64_t u64_handler_type = (static_cast<uint64_t>(handler_type) << 32);
	epoll_info.data.u64 = u64_handler_type + u64_socket_fd;

	//	EPOLLIN: we are interested only in request_for_connection events (sort of reading)
	// epoll_info.events = EPOLLIN;
	epoll_info.events = epoll_type;	

	//	at this point we add sockets, which do only listening - that is they are for reading(accept) event
	//	epoll_ctl() copies info from 'epoll_info', so we don't need to keep the epoll_event obj (=epoll_info)

	if (epoll_ctl(epoll_fd_, EPOLL_CTL_ADD, socket_fd, &epoll_info) != 0)
		throw AppException(prog_name_, "System error: Can't add a new entry to epoll" + ft_code_location(__FILE__, __LINE__));
}

void	AppRun::delete_from_epoll(const int socket_fd)
{
	//	if client_socket_fd = -1 then epoll will ignore it, we may keep fd=-1 in the interest list
	int check_epoll = epoll_ctl(epoll_fd_, EPOLL_CTL_DEL, socket_fd, NULL);
	#ifndef NDEBUG
	if (check_epoll == -1)
		std::cerr << TAB_ << prog_name_ << YELLOW_BOLD_HIGH_ << WARNING_ << DEFAULT_COLOR_ \
				<< "System error: Can't delete socket fd from epoll interest list: fd = " << socket_fd << '\n';
	#else
		(void)check_epoll;
	#endif
}