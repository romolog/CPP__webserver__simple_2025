/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_40_kill_timeout.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:27:24 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"

#include <iostream>
#include <signal.h>

void	AppRun::kill_timeout_processes_handlers(void)
{
	t_socketfd_handler_map::iterator it = app_handlers_.begin();
	t_socketfd_handler_map::iterator it_dup = it;
	t_socketfd_handler_map::iterator it_end = app_handlers_.end();
	
	while(it != it_end)
	{
		
		++it_dup;
		int is_expired = it->second->is_expired();
		
		if (is_expired && it->second->handler_status_ == CGI_WAITING_)
		{
			
			// stop CGI (SIGKILL can't be ignored or catched)
			int check_kill = kill(it->second->cgi_pid_, SIGKILL);
			if (check_kill == -1)
				std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ << "Can't stop CGI with pid: " << it->second->cgi_pid_ << ft_code_location(__FILE__, __LINE__) << '\n';
			
			
			set_error_response(it->second, "Timeout " + ft_code_location(__FILE__, __LINE__), 408);
			switch_to_prepare_response(it->second);


		}
		else if (is_expired)
		{
			close_connection(it->first);
		}

		it = it_dup;
	}	

}