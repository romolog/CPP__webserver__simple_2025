/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_ServerConfig.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:17:31 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 20:21:35 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_ServerConfig.hpp"

ServerConfig::ServerConfig() :
	Config(),
	cgi_rewrite_("")
{}

ServerConfig::~ServerConfig()
{}