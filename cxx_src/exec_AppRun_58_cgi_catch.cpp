/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_58_cgi_catch.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 19:42:01 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/wait.h>


void	AppRun::cgi_catch(Handler *handler_ptr)
{

	
	// HTTP_Request& request = handler_ptr->http_request_;
	// HTTP_Response& response = handler_ptr->http_response_;
	CGI_Response& cgi_response = handler_ptr->cgi_response_;

	int cgi_pid = handler_ptr->cgi_pid_;

	//	non-blocking check if CGI is ready
	//		https://stackoverflow.com/questions/33508997/waitpid-wnohang-wuntraced-how-do-i-use-these
	int stat_val;
	int	check_waitpid = waitpid(cgi_pid, &stat_val, WNOHANG);
	
	//	CGI is still in progress
	if (!check_waitpid)
		return ;


	//	waitpid error
	//	CGI exited with FAILURE
	//	CGI was terminated by a signal
	if ((check_waitpid == -1) || \
		(check_waitpid == cgi_pid && WIFEXITED(stat_val) && WEXITSTATUS(stat_val) != EXIT_SUCCESS) || \
		(check_waitpid == cgi_pid && WIFSIGNALED(stat_val))) 
	{
		// 500 Internal Server Error
		set_error_response(handler_ptr, "System error: Failed CGI: " + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return ;
	}

	// CGI exited with SUCCESS
	
	//	arrange BUFFER for reading CGI output
	std::vector<char> buff_vector;
	buff_vector.reserve(SERVER_BUFFER_SIZE_ + 1);
	char* buff = buff_vector.data();
	
	//	read CGI output and close socket
	int check_read = read(handler_ptr->cgi_socket_out_, buff, SERVER_BUFFER_SIZE_);
	int check_close = close(handler_ptr->cgi_socket_out_);

	if(check_read >= 0 && !check_close)
	{
		buff[check_read] = '\0';
		cgi_response.response_str_ += buff;
		handler_ptr->handler_status_ = CGI_CATCHED_;
	}
	else
	{

		// 500 Internal Server Error
		set_error_response(handler_ptr, "System error: Failed read or close CGI output: " + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
	}

	//	CGI output is saved to "cgi_response.response_str_"

}
