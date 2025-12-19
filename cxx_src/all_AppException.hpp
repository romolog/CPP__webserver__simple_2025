/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppException.hpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 18:22:25 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:56:10 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <exception>
#include <stdexcept>
#include <string>

class AppException : public std::runtime_error
{
	public:
		explicit AppException(	const std::string& what_arg);

		explicit AppException(	const std::string& prog_name, \
								const std::string& err_message);

		explicit AppException(	const std::string& prog_name, \
								const std::string& err_message, \
								const std::string& token);

		explicit AppException(	const std::string& prog_name, \
								const std::string& err_message, \
								const std::string& file_name, \
								const std::string& line_num);
};