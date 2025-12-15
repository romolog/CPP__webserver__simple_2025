/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_65_prepare_post.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:21:44 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>

//	RFC POST:
//		https://www.rfc-editor.org/rfc/rfc9110#name-post

// PROCESSING POST:
//		https://www.atatus.com/blog/http-post-request-method/
//
//		target = FILE or DIR
//			DIR:
//				error
//			FILE non-existing:
//				upload allowed : new file is created
//				upload NOT allowed : error
//			FILE existing:
//				try to ADD request.BODY to file's content (vs PUT: try to REPLACE file's content by request.BODY)

//  EXAMPLE:
// POST /upload?upload_progress_id=12344 HTTP/1.1
// Host: localhost:3000
// Content-Length: 1325
// Origin: http://localhost:3000
// ... other headers ...
// Content-Type: multipart/form-data; boundary=----WebKitFormBoundaryePkpFF7tjBAqx29L

// ------WebKitFormBoundaryePkpFF7tjBAqx29L
// Content-Disposition: form-data; name="MAX_FILE_SIZE"

// 100000
// ------WebKitFormBoundaryePkpFF7tjBAqx29L
// Content-Disposition: form-data; name="uploadedfile"; filename="hello.o"
// Content-Type: application/x-object

// ... contents of file goes here ...
// ------WebKitFormBoundaryePkpFF7tjBAqx29L--


void	AppRun::prepare_post(Handler *handler_ptr)
{

	HTTP_Request& request = handler_ptr->http_request_;
	HTTP_Response& response = handler_ptr->http_response_;	
	ServerConfig& serv_config = handler_ptr->config_server_;
	Location& location = handler_ptr->config_location_;

	std::string root_server = serv_config.root_;
	std::string root_location = location.root_;
	std::string target = (root_location.empty() ? root_server : root_location) + request.uri_path_;

	// if target = Dir set ERROR_RESPONSE
	size_t target_len = target.size();
	if (target[target_len - 1] == '/')
	{

		set_error_response(handler_ptr, "POST: Invalid target: Dir" + ft_code_location(__FILE__, __LINE__), 400);
		prepare_error(handler_ptr);
		return ;
	}

	//	check if target does exist and switch accordingly
	int is_exist = !access(target.c_str(), F_OK); 
	int	is_body = !request.body_.empty();
	int is_uploade_allowed = location.upload_allowed_;
	if (!is_exist && is_body && is_uploade_allowed) // target doesn't exist and upload is allowed
	{

		std::ofstream ofs(target.c_str());
		if (!ofs.is_open())
		{

			set_error_response(handler_ptr, "POST: Invalid target: Dir" + ft_code_location(__FILE__, __LINE__), 500);
			prepare_error(handler_ptr);
			return ;
		}


		ofs << request.body_;
		ofs.close();
	}
	else if (!is_exist)  // target doesn't exist and upload is impossible (not allowed or body is empty)
	{

		set_error_response(handler_ptr, "POST: Invalid target: Doesn't exist and upload is not allowed" + ft_code_location(__FILE__, __LINE__), 400);
		prepare_error(handler_ptr);
		return ;
	}
	else // target exists
	{
		std::ofstream ofs(target.c_str(), std::ios::app);
		if (!ofs.is_open())
		{

			set_error_response(handler_ptr, "POST: Invalid target: Dir" + ft_code_location(__FILE__, __LINE__), 500);
			prepare_error(handler_ptr);
			return ;
		}


		ofs << request.body_ << std::flush;
		ofs.close();
	}

	
	std::string status_line = "HTTP/1." + ft_itoa(request.http_version_) + " 200 OK\r\n";
	std::string content_type = "";
	response.full_body_ = "";

	assemble_chunk_or_unchunk(handler_ptr, response, status_line, content_type);
	handler_ptr->handler_status_ = READY_TO_RESPONSE_;

}

