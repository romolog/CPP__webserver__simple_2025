/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_Config.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:17:31 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 19:44:44 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"

#include <map>
#include <string>

//	IMPORTANT: README:
//		config file for static web-site NGINX:
// 		https://docs.nginx.com/nginx/admin-guide/web-server/serving-static-content/

struct Config
{		
	public:
		int								autoindex_;				// Server and Location: default 0 NO_ for Server and -2 NOT_SET_ for Location
		std::string						cgi_ext_;				//	default ""
		int								client_max_body_size_;	// Server and Location: default -2 NOT_SET_ 
		std::map<int, std::string>		err_pages_;				// Server and Location: 
		std::string						index_page_;			// Server and Location:
		int								redirection_code_;		// Server and Location: default -2 NOT_SET_
		std::string						redirection_path_;		// Server and Location:
		std::string						root_;					// Server and Location: default ""
	public:
										Config			();

};
	
