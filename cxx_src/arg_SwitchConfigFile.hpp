/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_SwitchConfigFile.hpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 14:57:27 by rponomar          #+#    #+#             */
/*   Updated: 2025/07/13 22:33:34 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"

#include <string>

class ConfigSwitch
{
	public:
		std::string			config_file_name_;
	public:
		ConfigSwitch(const int argc, const char* argv_1, const std::string& prog_name);
};