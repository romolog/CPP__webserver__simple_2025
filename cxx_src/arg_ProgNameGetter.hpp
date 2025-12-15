/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_ProgNameGetter.hpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:19:51 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/10 14:56:05 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <string>

class ProgNameGetter
{
	private:
		std::string		prog_name_;
	public:
		std::string		name(void);
	public:
		ProgNameGetter(const char* prog_name);
};
