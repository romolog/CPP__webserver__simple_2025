/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_Config.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 16:17:31 by rponomar          #+#    #+#             */
/*   Updated: 2025/07/28 17:33:59 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "conf_Config.hpp"

Config::Config() :
	autoindex_(NO_),
	cgi_ext_(""),
	client_max_body_size_(NOT_SET_),
	index_page_(DEFAULT_INDEX_PAGE_),
	redirection_code_(NOT_SET_),
	root_("")
{}
	
