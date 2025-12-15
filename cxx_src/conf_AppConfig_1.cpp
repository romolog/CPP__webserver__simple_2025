/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_1.cpp                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/14 18:36:07 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>

#include "conf_AppConfig.hpp"

t_config_map	AppConfig::make_app_config(void)
{
	//	check if conf file is valid (may throw exception if: no file name, not a file)
	is_valid_file();
	
	//	convert file to string (may throw excpetion if: permission denied, empty)
	file_to_string();

	//	take away comments from the conf string (may throw exception if: results in empty string)
	eliminate_comments();

	//	split conf string into tokens (may throw exception if: isspace, results less than MIN_TOKENS_NUN tokens)
	string_to_tokens();

	//	convert tokens to lexems (may thrown exception if: invalid token)
	tokens_to_lexems();

	//	parse lexems to config (may thrown exception if: invalid syntax, incomplete or redundant configuration)
	lexems_to_config();

	return (app_config_);
}

std::set<uint64_t>	AppConfig::get_app_host_ports(void)
{
	return (app_host_ports_);
}