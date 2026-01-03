/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_HTTP_4_Response.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:40:49 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 17:05:31 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "exec_Epoll_Interface.hpp"

#include "wbs_defaults.hpp"
#include "wbs_typedef_enum.hpp"

#include <arpa/inet.h> // htons, inet_addr (or inet_pton)
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>


class HTTP_Response
{
	public:
		std::string		response_str_;	// final responce to send(), may be a 'chunk' or 'full-response'
		int				response_code_;
		int				error_; // default 0 = NO_
	public:
		std::string		full_body_;		// full responce body, may be chunked and send by parts till is empty
		int				is_chunked_; 	// default 0 = NO_
		int				is_final_chunk_;// default 0 = NO_
		std::string		content_type_;
	public:
		int				is_head_; // default 0 = NO_ not a HEAD response
	public:
						HTTP_Response	();
};

