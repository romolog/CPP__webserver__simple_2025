/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_Lexem.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:29:02 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 19:44:55 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_Lexem.hpp"

Lexem::Lexem(int type, std::string& content_str) :
	type_(type),
	content_str_(content_str)
{}
