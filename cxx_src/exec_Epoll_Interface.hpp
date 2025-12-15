/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_Epoll_Interface.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:17:34 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:52:59 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <sys/epoll.h>
#include <netinet/in.h> // sockaddr_in
#include <string>

class Epoll_Interface
{
	public:
		const std::string&	prog_name_;
	public:
		uint32_t			event_mask_;
	public:
		int					socket_fd_;
		sockaddr_in			sockaddr_in_;
		int					socket_option_switch_;
	public:
		uint64_t			host_port_; 
	public:
							Epoll_Interface				(const std::string& prog_name);
};
