/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch_Listener.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 18:41:25 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/25 16:37:24 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_HTTP_1_Listener.hpp"

#include "all_AppException.hpp"
#include "exec_HTTP_2_Handler.hpp"

#include <iostream>
#include <unistd.h>

#define SWITCH_ON_ 1

Listener::Listener(const std::string& prog_name) : Epoll_Interface(prog_name)
{}

