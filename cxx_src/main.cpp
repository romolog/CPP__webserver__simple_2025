/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/07 18:31:15 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:49:32 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wbs_defaults.hpp"
#include "wbs_err_messages.hpp"
#include "wbs_shell_colors.hpp"

#include "all_SignalHandler.hpp"
#include "all_AppException.hpp"

#include "arg_ProgNameGetter.hpp"
#include "arg_SwitchConfigFile.hpp"

#include "conf_AppConfig.hpp"
#include "conf_ServerConfig.hpp"

#include "exec_AppLaunch.hpp"

#include "exec_AppRun.hpp"

#include <iostream>
#include <string>
#include <vector>

int	main(int argc, char *argv[], char** env)
{
	SignalHandler::set_sig_catch();
	
	//	get prog name from argv[0] = "webserv: "
	ProgNameGetter prog_name_getter(argv[0]);
	std::string prog_name = prog_name_getter.name();

	
	try
	{
		//	argc = 2 exit(failure), argc = 1 config file = argv[1], argc = 0 config file = default
		ConfigSwitch config_switch(argc, argv[1], prog_name);
		std::string& config_file = config_switch.argv_or_default();

		//	parse configuration file, create config_map: key (pair HOST - PORT), value (pair SERV_NAME - SERV_CONF) 
		AppConfig app_config_maker(config_file, prog_name);
		app_config_maker.make_app_config();
		t_config_map& app_config = app_config_maker.app_config_;

		AppLaunch app_http_servers_launcher(app_config, prog_name);
		t_socketfd_listener_map& app_listeners = app_http_servers_launcher.launch_http_listeners();

		AppRun app_executor(app_config, app_listeners, prog_name, env);
		app_executor.main_loop();

		
	}
	catch (const std::runtime_error& rte)
	{
		std::cerr << rte.what() << std::endl;
		return (EXIT_FAILURE);
	}
	catch(const std::exception& e)
	{
		std::cerr << TAB_ << prog_name << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_  << "System error: " << e.what() << '\n';
		return (EXIT_FAILURE);		
	}
	catch (...)
	{
		std::cerr << TAB_ << prog_name << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_  << "Something goes unexpectedly wrong...\n";
		return (EXIT_FAILURE);
	}

	return (EXIT_SUCCESS);

}