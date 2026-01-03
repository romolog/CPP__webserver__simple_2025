/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_HTTP_2_Handler.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:40:49 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:02:20 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_HTTP_2_Handler.hpp"

#include "all_utils.hpp"

#include "wbs_shell_colors.hpp"
#include "wbs_err_messages.hpp"

#include <iostream>
#include <unistd.h>

Handler::Handler(const std::string& prog_name, ServerConfig& config_server) : 
	Epoll_Interface(prog_name),
	handler_status_(HANDLER_STATUS_NOT_SET_),
	timeout_set_(NO_),
	config_server_(config_server),
	cgi_pid_(CGI_PID_DFLT_),
	cgi_response_received_(NO_)
{}

// Handler::Handler() : Epoll_Interface("")
// {}


//	SET:
//		start_time_ : update
//		timeout_duration_ : set to value
void		Handler::set_timeout(t_ull timeout_duration)
{
	timeout_set_ = YES_;

	//	set start time for the further timeout check
	time(&start_time_);

	//	set start time for the further timeout check
	timeout_duration_ = timeout_duration;
}


//	RETURN:
//		0 : NOT expired (not expired or NOT SET timeout)
//		1 : expired
int		Handler::is_expired(void)
{
	if (timeout_set_ == NO_)
		return (0);
		
	time_t	now_time;
	time(&now_time);

	double lapsed_time_milisec = difftime(now_time, start_time_) * 1000;

	return (lapsed_time_milisec > static_cast<double>(timeout_duration_));
}

void		Handler::reset(void)
{
	timeout_set_ = NO_;

	HTTP_Request reset_request;
	http_request_ = reset_request;
	
	HTTP_Response reset_response;
	http_response_ = reset_response;

	CGI_Request reset_cgi_request;
	cgi_request_ = reset_cgi_request;

	CGI_Response reset_cgi_response;
	cgi_response_ = reset_cgi_response;

	cgi_pid_ = -1;
	cgi_socket_out_ = -1;
	cgi_response_received_ = NO_;

	cgi_response_headers_.clear();
	cgi_response_body_.clear();
	
}