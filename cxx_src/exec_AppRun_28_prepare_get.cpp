/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_28_prepare_get.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:28:23 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>


void	AppRun::prepare_get(Handler *handler_ptr)
{

	HTTP_Request& request = handler_ptr->http_request_;
	HTTP_Response& response = handler_ptr->http_response_;
	ServerConfig& serv_config = handler_ptr->config_server_;
	Location& location = handler_ptr->config_location_;

	std::string& root_server = serv_config.root_;
	std::string& root_location = location.root_;
	std::string& root = (root_location.empty() ? root_server : root_location);
	std::string& location_path = location.path_;
	std::string status_code = " 200 OK\r\n";
	std::string target = request.uri_path_;
	
	if (!root.empty())
	{
		std::string& uri = request.uri_path_;
		uri = uri.substr(location_path.length());
		target = root + uri;
	}

	// if INDEX page is set, than we will add it to target just in case: 
	//		root = a/b/c/
	//		location = /non_exist_folder/
	//		index = ../file
	//		result = a/b/c/non_exist_folder/../file
	//		WHICH IS THE EXISTING FILE

	int is_dir;
	int	is_regular_file;
	long long file_size;
	bool no_acces = !access(target.c_str(), F_OK) && access(target.c_str(), R_OK);
	if (no_acces)
	{
		std::string err_msg =	"GET: Invalid required resource: " + \
								target.substr(0, ERROR_INPUT_SNIPET_LEN_) + \
								ft_code_location(__FILE__, __LINE__);
		set_error_response(handler_ptr, err_msg, 403);
		prepare_error(handler_ptr);
		return ;
	}

	if (ft_get_file_info(is_dir, is_regular_file, file_size, target))
	{
		std::string err_msg =	"GET: Invalid required resource: " + \
								target.substr(0, ERROR_INPUT_SNIPET_LEN_) + \
								ft_code_location(__FILE__, __LINE__);
		set_error_response(handler_ptr, err_msg, 404);
		prepare_error(handler_ptr);
		return ;
	}


	// AUTOINDEX overrides DEFAULT INDEX PAGE for DIRECTORY request
	if (is_dir && (location.autoindex_ == YES_ || (serv_config.autoindex_ == YES_ && location.autoindex_ == NOT_SET_)))
	{
		//	directory listing
		//	only regular files and directories are supported
		//	only listing for current directory supported, listing of subdirectories is not supported
		
			DIR *dir = opendir(target.c_str());
			if (!dir)
			{
				set_error_response(handler_ptr, "GET: required resource is a dir and can't open it: " + target.substr(0, ERROR_INPUT_SNIPET_LEN_), 404);
				prepare_error(handler_ptr);
				return ;
			} 

			// print all the files and directories within directory
			std::stringstream ss_dirlist;
			ss_dirlist << HTML_START_ << "<h3>DIRECTORY LISTING:</h3>\n<ul>";
			struct dirent *ent = readdir(dir);
			while (ent)
			{
				ss_dirlist << "<li> " << ent->d_name << "</li>\n";
				ent = readdir(dir);
			}
			closedir(dir);
			ss_dirlist << "</ul>";

			std::string status_line = "HTTP/1." + ft_itoa(request.http_version_) + " 200 OK\r\n";
			std::string content_type = "CONTENT-TYPE: text/html\r\n";
			std::string& body = response.full_body_;
			body = ss_dirlist.str();
			
			assemble_chunk_or_unchunk(handler_ptr, response, status_line, content_type);
			handler_ptr->handler_status_ = READY_TO_RESPONSE_;
	}
	else if (is_dir && location.index_page_.empty() && serv_config.index_page_.empty())
	{
		set_error_response(handler_ptr, "GET: required resource is a dir and autoindex is 'off': " + target.substr(0, ERROR_INPUT_SNIPET_LEN_) + ft_code_location(__FILE__, __LINE__), 404);
		prepare_error(handler_ptr);
		return ;
	}
	else
	{
		// if request is /a/b/c/ we try to deliver INDEX_PAGE
		if (is_dir)
		{


			std::string index_page(serv_config.index_page_);

			if (!location.index_page_.empty())
				index_page = location.index_page_;
			if (target[target.size() - 1] != '/')
				target += '/';
			if (target == "/")
				target.clear();
			target += index_page;


			bool no_acces = !access(target.c_str(), F_OK) && access(target.c_str(), R_OK);

			if (	no_acces || \
					ft_get_file_info(is_dir, is_regular_file, file_size, target) || \
					!is_regular_file)
			{

				std::string err_msg = "GET: Invalid required resource: " + \
										target.substr(0, ERROR_INPUT_SNIPET_LEN_) + \
										ft_code_location(__FILE__, __LINE__);
				
								
				if (no_acces && is_regular_file)
					set_error_response(handler_ptr, err_msg, 403);
				else
					set_error_response(handler_ptr, err_msg, 404);
				prepare_error(handler_ptr);
				return ;
			}
		}
		
		// will be response chunked
		//		status line + 
		//		header_content_len / header_chunked
		//		header_content_type // try first without it // README: https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Headers/Content-Type
		//		body

		std::string& body = response.full_body_;
		std::string err_msg;
		int check = ft_file_to_str(target, body, err_msg);
		if (check == -1)
		{
			// 404 Not Found
			set_error_response(handler_ptr, err_msg + ft_code_location(__FILE__, __LINE__), 404);
			prepare_error(handler_ptr);
			return ;
		}

		//	204 is not supported by CURL
		// if (body.empty())
		// 	status_code = "204 No Content\r\n";

		// status-line = HTTP-version SP status-code SP [ reason-phrase ]
		std::string status_line = "HTTP/1." + ft_itoa(request.http_version_) + status_code;
		std::string content_type = set_content_type(target);

		assemble_chunk_or_unchunk(handler_ptr, response, status_line, content_type);
		handler_ptr->handler_status_ = READY_TO_RESPONSE_;

	}	
}

