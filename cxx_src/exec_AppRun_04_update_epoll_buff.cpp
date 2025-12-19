/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_04_update_epoll_buff.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 19:59:10 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"

#include <iostream>

void AppRun::update_epoll_buffer(void)
{
	//	update max possible epoll events
	unsigned long long tmp_max = listeners_num_ + app_handlers_.size();

	if (tmp_max <= DEFAULT_EPOLL_EVENTS_MAX_NUM_)
		epoll_max_num_ = tmp_max;
	else
	{
		std::cerr << TAB_ << prog_name_ << YELLOW_BOLD_HIGH_ << WARNING_ << DEFAULT_COLOR_ << "Server is overloaded with events num: " << tmp_max << '\n';
		epoll_max_num_ = DEFAULT_EPOLL_EVENTS_MAX_NUM_;
	}

	//	do not want to allocate/reallocate/free memory manually for the (epoll_event arr[max_num])
	//	instead we use vector interface for this
	epoll_event_vector_.resize(epoll_max_num_);
	epoll_events_ptr_ = epoll_event_vector_.data();
}
