/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_utils_error_response.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:25:07 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include <iostream>

void	AppRun::set_error_response(Handler* handler_ptr, const std::string& log_stderr_message, const int error_code)
{
	std::cerr	<< TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ \
				<< log_stderr_message << '\n';

	// set client status, so it can't read and is ready to responce
	handler_ptr->handler_status_= ERROR_RESPONSE_;

	//	set "BAD REQUEST" responce code
	handler_ptr->http_response_.response_code_ = error_code;

	handler_ptr->set_timeout(TIMEOUT_FOR_SENDING_RESPONSE_);
}


void	AppRun::switch_to_prepare_response(Handler* handler_ptr)
{
	if (app_handlers_can_read_.count(handler_ptr->socket_fd_))
		delete_handler_from_map(handler_ptr->socket_fd_, app_handlers_can_read_);

	delete_from_epoll(handler_ptr->socket_fd_);

	if (!app_handlers_prepare_response_.count(handler_ptr->socket_fd_))
	{
		app_handlers_prepare_response_.insert(std::pair<uint32_t, Handler*>(handler_ptr->socket_fd_, handler_ptr));
	}
}
