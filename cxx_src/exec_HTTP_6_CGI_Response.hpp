/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_HTTP_6_CGI_Response.hpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:40:49 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:49:48 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "exec_Epoll_Interface.hpp"

#include "wbs_defaults.hpp"
#include "wbs_typedef_enum.hpp"

#include <arpa/inet.h> // htons, inet_addr (or inet_pton)
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>

typedef	unsigned long long t_ull;


class CGI_Response
{
	public:
		std::string				response_str_;	// catch output of CGI
		t_hash_table_headers	headers_;		
		std::string				body_; 			// body of CGI response
	public:
		std::string				content_len_;	// ready header 'Content-Length' based on body.size()
	public:
		std::string				content_type_;	// processed header 'Content-Type' of CGI-response, example: "text/html\r\n"
		std::string				ref_returned_;	// processed header 'Location' of CGI-response, example: "text/html\r\n"
		std::string				status_;		// default empty string // maybe default "200 OK\r\n", processed header 'Status' of CGI-response, example: "302 Found\r\n"
		int						code_; 			// default NOT_SET_
	public:
								CGI_Response	();

};

