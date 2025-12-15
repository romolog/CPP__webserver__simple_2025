/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_Location.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:03:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 20:21:50 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wbs_defaults.hpp"

#include "conf_Location.hpp"

Location::Location() : 
	Config(),
	path_(""),
	method_bits_(UINT32_MAX),
	upload_allowed_(NO_)
{
	autoindex_ = NOT_SET_;
}

Location::~Location() 
{}