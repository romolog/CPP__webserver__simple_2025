/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_AppException.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 18:19:02 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 18:19:18 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wbs_err_messages.hpp"
#include "wbs_shell_colors.hpp"

#include "all_AppException.hpp"

AppException::AppException(	const std::string& what_arg) :
	std::runtime_error(what_arg)
{}

AppException::AppException(	const std::string& prog_name, \
							const std::string& err_message) :
	std::runtime_error(TAB_ + prog_name + RED_BOLD_HIGH_ + ERROR_ + DEFAULT_COLOR_ + err_message)
{}

AppException::AppException(	const std::string& prog_name, \
							const std::string& err_message, \
							const std::string& token) :
	std::runtime_error(TAB_ + prog_name + RED_BOLD_HIGH_ + ERROR_ + DEFAULT_COLOR_ + err_message + ": " + token)
{}

AppException::AppException(	const std::string& prog_name, \
	const std::string& err_message, \
	const std::string& file_name, \
	const std::string& line_num) :
	std::runtime_error(TAB_ + prog_name + RED_BOLD_HIGH_ + ERROR_ + DEFAULT_COLOR_ + TAB_ + err_message + ": FILE: " + file_name + " : LINE: " + line_num)
{}