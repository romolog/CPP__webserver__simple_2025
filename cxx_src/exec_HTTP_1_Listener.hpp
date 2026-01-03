/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_Listener.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:41:25 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/25 16:35:37 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"

#include "exec_Epoll_Interface.hpp"

#include <arpa/inet.h> // htons, inet_addr (or inet_pton)
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>

class Listener : public Epoll_Interface
{
	public:
		Listener(const std::string& prog_name);
};

