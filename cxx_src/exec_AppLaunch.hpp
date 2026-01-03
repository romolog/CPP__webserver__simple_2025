/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppLaunch.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:48:42 by rponomar          #+#    #+#             */
/*   Updated: 2025/07/15 19:59:00 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "conf_AppConfig.hpp"
#include "exec_HTTP_1_Listener.hpp"

typedef std::map<int, Listener>					t_socketfd_listener_map;

class AppLaunch
{
//	RESULT ------------------------------------------------------------
	private:
		t_socketfd_listener_map						app_http_listeners_;
	public:
		t_socketfd_listener_map&					launch_http_listeners				(void);
//	INPUT -------------------------------------------------------------
	private:
		t_config_map&								app_config_;
		const std::string&							prog_name_;
		
//	TOOL -------------------------------------------------------------
	private:
		std::vector<int>							socket_fds_;


//	CTORS-DTORS-ETC ---------------------------------------------------
	public:
		AppLaunch(t_config_map& app_config, const std::string& prog_name);
		~AppLaunch();
};