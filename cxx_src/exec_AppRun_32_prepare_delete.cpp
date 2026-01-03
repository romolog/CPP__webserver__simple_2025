/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_32_prepare_delete.cpp                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:55:24 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <cstdio>
#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>

//	RFC DELETE:
//		https://www.rfc-editor.org/rfc/rfc9110#name-delete

// PROCESSING DELETE:
//
//		target = FILE or DIR
//			DIR:
//				error
//			FILE non-existing:
//				error
//			FILE existing:
//				try to delete


void	AppRun::prepare_delete(Handler *handler_ptr)
{
	HTTP_Request& request = handler_ptr->http_request_;
	HTTP_Response& response = handler_ptr->http_response_;	
	ServerConfig& serv_config = handler_ptr->config_server_;
	Location& location = handler_ptr->config_location_;

	std::string root_server = serv_config.root_;
	std::string root_location = location.root_;
	std::string target = (root_location.empty() ? root_server : root_location) + request.uri_path_;

	// C++11 : string.back() 
	// if target = Dir set ERROR_RESPONSE
	if (target[target.size() - 1] == '/')
	{
		set_error_response(handler_ptr, "DELETE: Invalid target: Dir" + ft_code_location(__FILE__, __LINE__), 404);
		prepare_error(handler_ptr);
		return ;
	}

	int is_dir;
	int	is_regular_file;
	long long file_size;
	(void) ft_get_file_info(is_dir, is_regular_file, file_size, target);
	(void) file_size;
	(void) is_dir;
	
	int check_delete = -1;
	if (is_regular_file)
		check_delete = std::remove(target.c_str());
	if (check_delete == -1)
	{
		set_error_response(handler_ptr, "DELETE: Can't delete file" + \
						ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return ;
	}

	std::string status_line = "HTTP/1." + ft_itoa(request.http_version_) + " 200 OK\r\n";
	std::string content_type = "";
	response.full_body_ = "";

	assemble_chunk_or_unchunk(handler_ptr, response, status_line, content_type);
	handler_ptr->handler_status_ = READY_TO_RESPONSE_;
}

