/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_03_start_epoll.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:02:52 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"

void	AppRun::start_epoll(void)
{
	epoll_fd_ = epoll_create(1);
	if (epoll_fd_ < 0)
		throw AppException(prog_name_, "System error: Can't create epoll");

}

void	AppRun::populate_epoll_with_listeners(void)
{

	t_socketfd_listener_map::iterator it = app_listeners_.begin();
	t_socketfd_listener_map::iterator it_end = app_listeners_.end();
	for (; it != it_end; ++it)
		add_to_epoll(it->first, HANDLER_LISTEN_, EPOLLIN);
}
