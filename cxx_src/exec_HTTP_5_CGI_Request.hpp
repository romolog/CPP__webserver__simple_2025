/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_HTTP_5_CGI_Request.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:40:49 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/16 21:44:43 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "exec_Epoll_Interface.hpp"

#include "wbs_defaults.hpp"
#include "wbs_typedef_enum.hpp"
#include "conf_Location.hpp"


#include <arpa/inet.h> // htons, inet_addr (or inet_pton)
#include <map>
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>
#include <string>
#include <vector>

class CGI_Request
{
	public:
		char**						cgi_env_;
		std::vector<std::string>	cgi_env_string_vector_;
		std::vector<char*>			cgi_env_c_str_vector_;
	public:	
		void						reset					(void);

};

