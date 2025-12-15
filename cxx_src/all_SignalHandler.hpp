/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_SignalHandler.hpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:14:24 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 18:31:37 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <csignal>


// since C++11 : std::sig_atomic_t (#include <csignal>)
class SignalHandler
{
	public:
		static volatile sig_atomic_t 		signum_; // default 0
	public:
		static void							sig_catch				(int signum);
		static void							set_sig_catch			(void);
		static void							set_sig_default			(void);		

};

