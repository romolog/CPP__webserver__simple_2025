/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_33_close_all_connections.cpp           :+:      :+:    :+:   */
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


void	AppRun::close_all_connections(void)
{
	t_socketfd_handler_map::iterator it = app_handlers_.begin();
	t_socketfd_handler_map::iterator it_dup = it;
	t_socketfd_handler_map::iterator it_end = app_handlers_.end();
	
	while(it != it_end)
	{		
		++it_dup;	
		close_connection(it->first);
		it = it_dup;
	}
}

