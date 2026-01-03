/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_Epoll_Interface.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 22:17:29 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:53:09 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_Epoll_Interface.hpp"

#include "all_AppException.hpp"

Epoll_Interface::Epoll_Interface(const std::string& prog_name) : prog_name_(prog_name)
{}