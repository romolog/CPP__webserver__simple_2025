/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_5_parsers_throw_exception.cpp  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:04:18 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"
#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>

void	AppConfig::throw_exception(std::vector<Lexem>::iterator& , std::vector<Lexem>::iterator&, std::vector<int>&, ServerConfig& )
{	

	throw AppException(prog_name_, "Something goes wrong: Wrong parser is called");
}

void	AppConfig::throw_exception(std::vector<Lexem>::iterator& , std::vector<Lexem>::iterator&, std::vector<int>&, Location& )
{	

	throw AppException(prog_name_, "Something goes wrong: Wrong parser is called");
}
