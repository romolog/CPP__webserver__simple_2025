/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppLaunch.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 19:48:42 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 19:56:19 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppLaunch.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <cassert>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>

AppLaunch::AppLaunch(t_config_map& app_config, const std::string& prog_name) :
	app_config_(app_config),
	prog_name_(prog_name)
{}

AppLaunch::~AppLaunch()
{
	t_socketfd_listener_map::iterator it = app_http_listeners_.begin();
	t_socketfd_listener_map::iterator it_end = app_http_listeners_.end();
	for(; it != it_end; ++it)
	{
		if (it->first >= 0)
		{
			int check_close = close(it->first);
			if (check_close == -1)
				std::cerr << "\tAppLaunch: Error: Can't close socket fd" << it->first << '\n';	
		}
		delete (it->second);
	}
}


t_socketfd_listener_map&	AppLaunch::launch_http_listeners(void)
{
	
	t_config_map::iterator it = app_config_.begin();
	t_config_map::iterator it_dup = it;
	t_config_map::iterator it_end = app_config_.end();

	// for (; it != it_end; ++it)
	while (it != it_end)
	{
		++it_dup;
		//	open socket fd
		//		AF_INET : IPv4
		//		kind of socket: Socket-Stream
		//		protocol: IPPROTO_TCP
		// new_http_server->socket_fd_ = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		//	SUBJECT REQUEST FOR NON-BLOCKING SOCKETS:
		//	"Because you have to use non-blocking file descriptors ..."
		//			Using O_NONBLOCK flag as bitwise OR saves extra calls to fcntl(2) to achieve the same result. 
		int new_listener_socket_fd = socket(AF_INET, SOCK_STREAM | O_NONBLOCK | O_CLOEXEC, IPPROTO_TCP);
		if (new_listener_socket_fd == -1)
			throw AppException(prog_name_, "System error: Can't open SOCK_STREAM socket fd under protocol IPPROTO_TCP");
		
		//	if above is success we create new HTTP_LISTENER and pass it the open socket fd
		Listener* new_http_listener = new Listener(prog_name_);
		new_http_listener->host_port_ = it->first;
		new_http_listener->socket_fd_ = new_listener_socket_fd;
		
		//	memset with 0  sockaddr_in.(unsigned char sin_zero[0])
		//	this is to make valgrind happy, that everything is initialized
		char* sinzero_ptr = reinterpret_cast<char *>(&(new_http_listener->sockaddr_in_.sin_zero));
		ft_memset(sinzero_ptr, 0, sizeof(new_http_listener->sockaddr_in_.sin_zero));
		
		//	set IPv4 family of IP addresses
		new_http_listener->sockaddr_in_.sin_family = AF_INET;
		
		//	set host
		uint32_t host = (it->first >> 32);
		new_http_listener->sockaddr_in_.sin_addr.s_addr = htonl(host);
		
		//	set port
		uint16_t port = ((it->first << 32) >> 32); 
		new_http_listener->sockaddr_in_.sin_port = htons(port);

		//	Bind socket and host-port (sockaddr_in) 
		sockaddr* sockaddr_ptr = reinterpret_cast<sockaddr*>(&(new_http_listener->sockaddr_in_));

		//	Safety?... - no we don't care and we will use setsockopt() to rerun server smoothly
		//	SAFETY RECOMMENDATIONS:
		// https://learn.microsoft.com/en-us/windows/win32/winsock/using-so-reuseaddr-and-so-exclusiveaddruse
		// //	SOL_SOCKET		- 	further options will be applicable for socket level
		// //	SO_REUSEADDR	-	Allows other sockets to bind() to this port, unless there is an active listening socket bound to the port already. 
		// //						This enables you to get around those “Address already in use” error messages when you try to restart your server after a crash.		
		int switch_on_off = 1;
		int check_setsockopt = setsockopt(new_http_listener->socket_fd_, SOL_SOCKET, SO_REUSEADDR, &switch_on_off, sizeof(switch_on_off));
		if (check_setsockopt == -1)
			std::cout << TAB_ << prog_name_ << YELLOW_BOLD_HIGH_ << WARNING_ << DEFAULT_COLOR_ << "System error: Can't set socket option SO_REUSEADDR\n";

		int check_bind = bind(new_http_listener->socket_fd_, sockaddr_ptr, sizeof(new_http_listener->sockaddr_in_));
		if (check_bind == -1)
		{
			int check_close = close(new_http_listener->socket_fd_);
			if (check_close == -1)
				std::cerr << "\tAppLaunch: Error: Can't close socket fd" << new_http_listener->socket_fd_ << '\n';
			delete new_http_listener;
			std::cout << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ << "System error: Can't bind 'Listener' to host: " << host << " port: " << port << ft_code_location(__FILE__, __LINE__) <<  '\n';
			app_config_.erase(it);
		}
		else
		{
			//	Make socket listen
			int listen_check = listen(new_http_listener->socket_fd_, DEFAULT_BACKLOG_);
			if (listen_check == -1)
			{
				int check_close = close(new_http_listener->socket_fd_);
				if (check_close == -1)
					std::cerr << "\tAppLaunch: Error: Can't close socket fd" << new_http_listener->socket_fd_ << '\n';			
				delete new_http_listener;
				throw AppException(prog_name_, "System error: Can't make socket listen");
			}
			app_http_listeners_.insert(std::pair<int, Listener*>(new_http_listener->socket_fd_, new_http_listener));
			std::cout << "\t\tNEW SERVER " << (((it->first) << 32) >> 32) << " LISTENS TO FD:\t" << new_listener_socket_fd << std::endl;
		}
		
		it = it_dup;
	}
	
	if (app_http_listeners_.empty())
		throw AppException(prog_name_, "Can't bind any host-port: Try to relaunch in a few minutes or change configuration");

	return (app_http_listeners_);
}
