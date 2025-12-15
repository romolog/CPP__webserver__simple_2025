/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_Location.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 20:03:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 20:21:44 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"
#include "conf_Config.hpp"

#include <stdint.h>
#include <string>
#include <vector>
#include <map>


struct Location : public Config
{

	public:
		std::string		path_;
	public:
		uint32_t		method_bits_; // DEFAULT: UINT32_MAX : all methods allowed (each method has its bit: 1 - allowed, 0 - not allowed)				
	public:
		int				upload_allowed_;	// default 0 = NO_
	public:
						Location			();
						~Location			();
};
