/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_HTTP_2_Handler.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:40:49 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:51:38 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"
#include "wbs_typedef_enum.hpp"

#include "exec_Epoll_Interface.hpp"

#include "conf_ServerConfig.hpp"
#include "conf_Location.hpp"
#include "exec_HTTP_3_Request.hpp"
#include "exec_HTTP_4_Response.hpp"
#include "exec_HTTP_5_CGI_Request.hpp"
#include "exec_HTTP_6_CGI_Response.hpp"

#include <arpa/inet.h> // htons, inet_addr (or inet_pton)
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>

class Handler : public Epoll_Interface
{
	public:
		int							handler_status_; // enum HandlerStatuses
		int							timeout_set_; // default NO_ = 0
		t_ull						timeout_duration_;
		time_t						start_time_;
	public:
		ServerConfig&				config_server_; 
		Location					config_location_;
		HTTP_Request				http_request_;
		HTTP_Response				http_response_;
		CGI_Request					cgi_request_;
		CGI_Response				cgi_response_;		
	public:
		int							cgi_pid_; // default -42
		int							cgi_socket_out_;	// to catch CGI output from child process, initialized by socketpair()
		int							cgi_response_received_; // default 0 = NO_
		t_hash_table_headers		cgi_response_headers_; // ToDo : maybe we need an extra classes CGI_Request and CGI_Response to store all this stuff ?..
		std::string					cgi_response_body_;
	public:
		void						reset				(void);
		public:
		void						set_timeout 		(t_ull timeout_duration);	
		int							is_expired 			(void);
	public:
									Handler			(const std::string& prog_name, ServerConfig& config_server);
};
	
