/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_SwitchConfigFile.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:57:27 by rponomar          #+#    #+#             */
/*   Updated: 2025/07/13 22:36:09 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wbs_err_messages.hpp"
#include "wbs_shell_colors.hpp"

#include "arg_SwitchConfigFile.hpp"

#include "all_AppException.hpp"

#include <iostream>
#include <cstdlib>
#include <cstddef>

#define ARG_NOT_ONE_ "More than 1 arg passed"
#define DEFAULT_CONFIG_MESSAGE_ "No config file passed as arg: Default configuration will be used"


ConfigSwitch::ConfigSwitch(const int argc, const char* argv_1, const std::string& prog_name)
{
	if (argc > 2)
		throw AppException(prog_name, ARG_NOT_ONE_);
	else if (argc == 2)
		config_file_name_ = argv_1;
	else
	{
		std::cerr 	<< TAB_ << prog_name << WARNING_ \
					<< DEFAULT_CONFIG_MESSAGE_ << std::endl;
		config_file_name_ = DEFAULT_CONFIG_FILE_;
	}
}
