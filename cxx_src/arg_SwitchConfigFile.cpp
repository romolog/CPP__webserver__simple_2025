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


ConfigSwitch::ConfigSwitch(const int argc, const char* argv_1, const std::string& prog_name) :
	argc_(argc),
	argv_1_(argv_1),
	prog_name_(prog_name)
{}

std::string&			ConfigSwitch::argv_or_default(void)
{
	if (argc_ > 2)
	{
		std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ << ARG_NOT_ONE_ << std::endl;		
		throw AppException(prog_name_, ARG_NOT_ONE_);
	}
	else if (argc_ == 2)
		config_file_name_ = argv_1_;
	else
	{
		std::cerr << TAB_ << prog_name_ << YELLOW_BOLD_HIGH_ << WARNING_ << DEFAULT_COLOR_ << DEFAULT_CONFIG_MESSAGE_ << std::endl;
		config_file_name_ = DEFAULT_CONFIG_FILE_;
	}
	return (config_file_name_);
}