/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:53:44 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"
#include "wbs_typedef_enum.hpp"

#include "exec_AppLaunch.hpp"
#include "exec_HTTP_2_Handler.hpp"

#include <map>
#include <string>
#include <sys/epoll.h>
#include <vector>

typedef	std::map<int, Handler*> 		t_socketfd_handler_map;

class AppRun
{
//	RESULT -------------------------------------------------------------
	public:
		void							main_loop			(void);
//	INPUT -------------------------------------------------------------
	private:
		t_config_map&					app_config_;
		t_socketfd_listener_map&		app_listeners_;
		const std::string&				prog_name_;
		char**							env_;
//	TOOL -------------------------------------------------------------
	private:
		int								do_exit_;
	private:
		t_socketfd_handler_map			app_handlers_;
		t_socketfd_handler_map			app_handlers_can_read_;
		t_socketfd_handler_map			app_handlers_prepare_response_;		
		t_socketfd_handler_map			app_handlers_can_response_;				
	private:
		int								epoll_fd_;
	private:
		int								epoll_count_;
		epoll_event*					epoll_events_ptr_;
		std::vector<epoll_event>		epoll_event_vector_;
		size_t							epoll_max_num_;
		size_t							listeners_num_;
	private:
		std::map<int, std::string>		response_status_codes_;
	private:	
		void							start_epoll								(void);
		void							add_to_epoll							(const int socket_fd, const int handler_type, uint32_t epoll_type);
		void							delete_from_epoll						(const int socket_fd);
	
	private:
		void							populate_epoll_with_listeners			(void);

	private:
		void							update_epoll_buffer						(void);
		void							handle_epoll_events						(void);
		void								accept_connection					(const int server_socket_fd);
		int									read_request						(const int handler_socket_fd);
		int										read_from_socket_to_buffer		(const int handler_socket_fd, Handler* handler_ptr, char* buffer);
		int										parse_request_line				(Handler* handler_ptr, std::string& request);
		int										parse_headers					(const int handler_socket_fd, Handler* handler_ptr, std::string& request);
		int											read_headers_trailers		(std::string& input_str, Handler* handler_ptr, std::string& err_msg, t_hash_table_headers& hash_table, const std::string& delimiter);			
		int										parse_uri						(Handler* handler_ptr, const std::string& uri);
		int										parse_header_host				(Handler* handler_ptr);
		int										assign_location_to_request		(Handler* handler_ptr);
		int										parse_body						(Handler* handler_ptr, std::string& request);
		int									send_response						(const int handler_socket_fd);
		void								close_connection					(const int handler_socket_fd);
	private:
		void									set_error_response				(Handler* handler_ptr, const std::string& log_stderr_message, const int error_code);
		void									switch_to_prepare_response		(Handler* handler_ptr);
		void									delete_handler_from_map			(const int handler_socket_fd, t_socketfd_handler_map& handler_map);
		std::string								set_content_type				(const std::string& file_name);
	private:
		void							kill_timeout_processes_handlers			(void);
	private:
		void							prepare_response						(void);	
		void									assemble_chunk_or_unchunk		(Handler* handler_ptr, HTTP_Response& response, const std::string& status_line, const std::string& content_type);
		void								prepare_error						(Handler* handler_ptr);
		void									default_error_body				(const int error_code, const std::string& error_page_path, std::string& error_body);
		void								prepare_chunk						(Handler* handler_ptr);
		void								prepare_redirect					(Handler* handler_ptr);		
		void								prepare_cgi							(Handler* handler_ptr);				
		void									cgi_launch						(Handler* handler_ptr);
		void										set_cgi_vars_to_env			(HTTP_Request& request, CGI_Request& cgi_request);
		void									cgi_catch						(Handler* handler_ptr);
		int											process_cgi_response		(Handler* handler_ptr);		
		void								prepare_delete						(Handler* handler_ptr);
		void								prepare_get							(Handler* handler_ptr);
		void								prepare_post						(Handler* handler_ptr);


	public:
										AppRun									(t_config_map& app_config, t_socketfd_listener_map& app_listeners, const std::string& prog_name, char** env);
};

