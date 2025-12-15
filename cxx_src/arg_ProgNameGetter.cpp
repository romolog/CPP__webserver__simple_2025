/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_ProgNameGetter.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:19:51 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/10 14:55:58 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "arg_ProgNameGetter.hpp"

ProgNameGetter::ProgNameGetter(const char* prog_name) : prog_name_(prog_name)
{}		

std::string	ProgNameGetter::name(void)
{
	std::string::size_type slash_pos_after = prog_name_.rfind('/') + 1;
	std::string::size_type len = prog_name_.size() - slash_pos_after;	
	prog_name_ = prog_name_.substr(slash_pos_after, len);
	prog_name_ += ": ";
	return (prog_name_);
}

