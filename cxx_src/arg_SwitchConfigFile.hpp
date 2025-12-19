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
	private:
		int					argc_;
		const char*			argv_1_;
		const std::string&	prog_name_;
		std::string			config_file_name_;
	public:
		std::string&			argv_or_default(void);
	public:
		ConfigSwitch(const int argc, const char* argv_1, const std::string& prog_name);
};