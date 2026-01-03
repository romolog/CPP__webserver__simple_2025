/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_utils_delete_handler_from_map.cpp      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:54:30 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"

#include <iostream>

void	AppRun::delete_handler_from_map(const int handler_socket_fd, t_socketfd_handler_map& handler_map)
{
	if (handler_map.count(handler_socket_fd))
	{
		int check_map = handler_map.erase(handler_socket_fd);	
		if (!check_map)
			throw AppException(prog_name_, "System error: Can't erase handler from a map");
	}
}

void	AppRun::delete_handler_from_map(const int handler_socket_fd, t_socketfd_handler_ptr_map& handler_map)
{
	if (handler_map.count(handler_socket_fd))
	{
		int check_map = handler_map.erase(handler_socket_fd);	
		if (!check_map)
			throw AppException(prog_name_, "System error: Can't erase handler from a map");
	}
}