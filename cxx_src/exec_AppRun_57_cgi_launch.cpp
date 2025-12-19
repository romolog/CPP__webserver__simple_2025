/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_57_cgi_launch.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:28:09 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_SignalHandler.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <cstdio>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/wait.h>


#define CHILD 0

//	IMPORTANT: README:
//		https://www.garshol.priv.no/download/text/http-tut.html

//	CGI HOW TO PASS PARAMETERS:
//		https://en.wikipedia.org/wiki/Common_Gateway_Interface
//		The CGI specification defines how additional information passed with the request is passed to the script. 
//		The Web server creates a subset of the environment variables passed to it and adds details pertinent to the HTTP environment. 
//		For instance, if a slash and additional directory name(s) are appended to the URL immediately after the name of the script (in this example, /with/additional/path), 
//		then that path is stored in the PATH_INFO environment variable before the script is called. 
//		If parameters are sent to the script via an HTTP GET request (a question mark appended to the URL, 
//		followed by param=value pairs; in the example, ?and=a&query=string), then those parameters are stored in the QUERY_STRING environment variable before the script is called. 
//		Request HTTP message body, such as form parameters sent via an HTTP POST request, are passed to the script's standard input. 
//		The script can then read these environment variables or data from standard input and adapt to the Web browser's request.[8]
//
//	CGI HOW TO PASS PARAMETERS: IMPORTAT: README
//		https://www.rfc-editor.org/rfc/rfc9110#name-application-handling-of-fie


//		launch of CGI process, the result will be catched at later step by cgi_catch()
//		launch and catch are splited for the purpose of parallel execution of all CGI processes
void	AppRun::cgi_launch(Handler *handler_ptr)
{
	
	HTTP_Request& request = handler_ptr->http_request_;
	CGI_Request& cgi_request = handler_ptr->cgi_request_;


	ServerConfig& serv_config = handler_ptr->config_server_;
	Location& location = handler_ptr->config_location_;

	//	target (file or dir) = root + uri_path
	//		root is inherited from 'Server' level, if there is no 'Location' level root
	std::string root_server = serv_config.root_;
	std::string root_location = location.root_;
	std::string target = (root_location.empty() ? root_server : root_location) + request.uri_path_;


	std::string::size_type dir_end_pos = target.rfind('/');
	std::string target_dir;
	if (dir_end_pos != std::string::npos)
		target_dir = target.substr(0, dir_end_pos + 1);

	int is_executable = !access(target.c_str(), X_OK);
	
	// check if target is an executable file and permision is granted, otherwise set ERROR_RESPONCE: 404 NOT FOUND
	int is_dir = -1;
	int	is_regular_file = -1;
	long long file_size = -1;
	int file_err = ft_get_file_info(is_dir, is_regular_file, file_size, target);

	if (file_err || is_dir == 1 || !is_executable)
	{

		std::string err_msg = "GET: Invalid required resource " + target.substr(0, ERROR_INPUT_SNIPET_LEN_) + ft_code_location(__FILE__, __LINE__);
		set_error_response(handler_ptr, err_msg, 404);
		prepare_error(handler_ptr);
		return ;
	}



	//	int socketpair(int domain, int type, int protocol, int socket_fds[2])
	//		AF_UNIX - domain supported by LINUX (also AF_TIPC since Linux 4.12)
	//		SOCK_STREAM - two-way connection
	//		protocol -	Normally only a single protocol exists to support a
    //					particular socket type within a given protocol family, in which
    //					case protocol can be specified as 0. 
	int	cgi_sockets_out[2];
	int check_socketpair = socketpair(AF_UNIX, SOCK_STREAM, 0, cgi_sockets_out);
	if (check_socketpair == -1)
	{

		// 500 Internal Server Error
		set_error_response(handler_ptr, "System error: Failed socketpair: " + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return ;
	}

	handler_ptr->cgi_socket_out_ = cgi_sockets_out[0];

	int	cgi_sockets_in[2];
	check_socketpair = socketpair(AF_UNIX, SOCK_STREAM, 0, cgi_sockets_in);
	if (check_socketpair == -1)
	{

		// 500 Internal Server Error
		set_error_response(handler_ptr, "System error: Failed socketpair: " + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return ;
	}


	int cgi_pid = fork();
	if (cgi_pid == -1)
	{

		// 500 Internal Server Error
		set_error_response(handler_ptr, "System error: Failed socketpair: " + ft_code_location(__FILE__, __LINE__), 500);
		prepare_error(handler_ptr);
		return ;
	}
	else if (cgi_pid == CHILD)
	{


		SignalHandler::set_sig_default();
		int check_close = close(cgi_sockets_out[0]);
		if (check_close == -1)
		{

			// PARENT will catch EXIT_FAILURE and set ERROR_RESPONSE
			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed close cgi_sockets[0]: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
			exit(EXIT_FAILURE);
		}



		int check_chdir = chdir(target_dir.c_str());
		if (check_chdir == -1)
		{

			// PARENT will catch EXIT_FAILURE and set ERROR_RESPONSE
			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed chdir: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
			exit(EXIT_FAILURE);
		}

		// put CGI variables to ENV
		set_cgi_vars_to_env(request, cgi_request);


		// set cgi_sockets[1] to be the STDOUT
		int check_dup2 = dup2(cgi_sockets_out[1], STDOUT_FILENO);
		if (check_dup2 == -1)
		{

			// PARENT will catch EXIT_FAILURE and set ERROR_RESPONSE
			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed dup2 STDOUT: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
			exit(EXIT_FAILURE);
		}

		check_close = close(cgi_sockets_out[1]);
		if (check_close == -1)
		{

			// PARENT will catch EXIT_FAILURE and set ERROR_RESPONSE
			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed close cgi_sockets[0]: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
			exit(EXIT_FAILURE);
		}

		// put request body to STDIN
		//		write to socket_in[1]: offset is EOF
		//		dup2(socket_in[0], STDIN) : offset is begin_of_file
		//		close socket_in[1]
		//		close socket_in[0]
		size_t body_size = request.body_.size() + 1;
		int check_write = write(cgi_sockets_in[1], request.body_.c_str(), body_size);
		if (check_write == -1 || check_write != static_cast<int>(body_size))
		{

			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed write body to socket: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
			exit(EXIT_FAILURE);
		}		

		check_dup2 = dup2(cgi_sockets_in[0], STDIN_FILENO);
		if (check_dup2 == -1)
		{

			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed dup2 STDIN: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
			exit(EXIT_FAILURE);
		}

		check_close = close(cgi_sockets_in[0]);
		if (check_close == -1)
		{

			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed close cgi_sockets_in[0]: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
			exit(EXIT_FAILURE);
		}

		check_close = close(cgi_sockets_in[1]);
		if (check_close == -1)
		{

			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed close cgi_sockets_in[1]: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
			exit(EXIT_FAILURE);
		}

		// EXECVE
		// char* const cgi_name_ptr = const_cast<char* const>(target.c_str());
		const char* temp_const_char = target.c_str();
		char* temp_char = const_cast<char* >(temp_const_char);
		char* const cgi_name_ptr = temp_char;
		char* const args[2] = {cgi_name_ptr, NULL};

		execve(cgi_name_ptr, args, cgi_request.cgi_env_);
		// PARENT will catch EXIT_FAILURE and set ERROR_RESPONSE
		std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_<< "CGI failed execve: " << strerror(errno) << " : " << ft_code_location(__FILE__, __LINE__) << '\n';
		exit(EXIT_FAILURE);
	}
	else // PARENT:
	{


		handler_ptr->cgi_pid_ = cgi_pid;

		int check_close = close(cgi_sockets_out[1]);
		if (check_close == -1)
		{
			
			std::cerr 	<< TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ \
						<< "CGI parent: System error: Failed close socket: " + ft_itoa(cgi_sockets_out[1]) + " : " + ft_code_location(__FILE__, __LINE__) << '\n';

		}

		check_close = close(cgi_sockets_in[0]);
		if (check_close == -1)
		{
			
			std::cerr 	<< TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ \
						<< "CGI parent: System error: Failed close socket: " + ft_itoa(cgi_sockets_out[1]) + " : " + ft_code_location(__FILE__, __LINE__) << '\n';
		}

		check_close = close(cgi_sockets_in[1]);
		if (check_close == -1)
		{
			
			std::cerr 	<< TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ \
						<< "CGI parent: System error: Failed close socket: " + ft_itoa(cgi_sockets_out[1]) + " : " + ft_code_location(__FILE__, __LINE__) << '\n';
		}

		handler_ptr->cgi_socket_out_ = cgi_sockets_out[0];
		
		handler_ptr->handler_status_ = CGI_LAUNCHED_;
	}
	
}

//	CIG STANDARD VARIABLES:
// 			https://www.cgi101.com/book/ch3/text.html
//			https://www6.uniovi.es/~antonio/ncsa_httpd/cgi/env.html
//			https://www.ibm.com/docs/en/netcoolomnibus/8.1.0?topic=scripts-environment-variables-in-cgi-script
//



//	sets STANDARD and HEADER varialbes for CGI to (char** env)
void  AppRun::set_cgi_vars_to_env(HTTP_Request& request, CGI_Request& cgi_request)
{


	// this we need for memory allocation even for large list of large vars
	std::vector<std::string>& cgi_vars_vector = cgi_request.cgi_env_string_vector_;


	// STANDARD VARIABLES FOR CGI
	cgi_vars_vector.push_back("QUERY_STRING=" + request.uri_query_str_);
	cgi_vars_vector.push_back("REQUEST_METHOD=" + request.method_str_);
	cgi_vars_vector.push_back("SERVER_PROTOCOL=HTTP/1." + ft_itoa(request.http_version_));
	cgi_vars_vector.push_back("PATH_INFO=HTTP/1." + ft_itoa(request.http_version_));		


	// HEADERS TO VARIABLES FOR CGI
	t_hash_table_headers::iterator it_header = request.headers_.begin();
	t_hash_table_headers::iterator it_header_end = request.headers_.end();

	for (; it_header != it_header_end; ++it_header)
	{
		//	header "Content-Length: 123"
		//	will be passed to CGI as env var
		//	CONTENT_LENGTH=123
		std::string new_var = "HTTP_" + it_header->first + "=" + it_header->second.second;
		new_var.replace(new_var.begin(), new_var.end(), '-', '_');
		std::transform(new_var.begin(), new_var.end(), new_var.begin(), ft_toupper);

		cgi_vars_vector.push_back(new_var);
	}

	char** env_ptr = env_;

	while (env_ptr && *env_ptr)
	{
		std::string new_var(*env_ptr);

		cgi_vars_vector.push_back(new_var);

		++env_ptr;
	}
	
	
	// this we need to convert into char** array
	std::vector<char*>& cgi_vars_cstr_vector = cgi_request.cgi_env_c_str_vector_;


	std::vector<std::string>::iterator it = cgi_vars_vector.begin();
	std::vector<std::string>::iterator it_end = cgi_vars_vector.end();
	for(; it != it_end; ++it)
		cgi_vars_cstr_vector.push_back(const_cast<char*>(it->c_str()));


	cgi_vars_cstr_vector.push_back(NULL);


	cgi_request.cgi_env_ = const_cast<char**>(cgi_vars_cstr_vector.data());


}

