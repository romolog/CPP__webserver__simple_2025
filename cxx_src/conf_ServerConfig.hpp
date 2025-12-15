/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_ServerConfig.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:17:31 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 20:21:39 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"

#include "conf_Config.hpp"
#include "conf_Location.hpp"


#include <stdint.h>
#include <cstdlib>
#include <map>
#include <string>
#include <set>
#include <vector>

//	IMPORTANT: README:
//		config file for static web-site NGINX:
// 		https://docs.nginx.com/nginx/admin-guide/web-server/serving-static-content/

struct ServerConfig : public Config
{
	public:
		std::vector< uint64_t >					host_ports_;	//	duplicates are not allowed, checked at App level by std::set app_host_ports
		std::string								server_name_;	// 	no virtual hosting : one server = one server_name (host_name)		
		std::string								cgi_rewrite_;	//	default "", only 'Server' level		
		
	public:
		std::map<std::string, Location>			locations_;		// resolving location checks path-prefix (no path-regex allowed), finds the longest matching prefix OR set BAD REQUEST error responce
	public:
												ServerConfig		();
												~ServerConfig		();

};
	
