/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   all_SignalHandler.cpp                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 18:14:28 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 18:24:46 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "all_SignalHandler.hpp"
#include "all_AppException.hpp"
#include "all_utils.hpp"

volatile sig_atomic_t	SignalHandler::signum_ = 0;

void SignalHandler::sig_catch(int signum)
{
	signum_ = signum;
}

// https://www.chromium.org/chromium-os/developer-library/reference/linux-constants/signals/
// https://man7.org/linux/man-pages/man7/signal.7.html
//	The signals SIGKILL and SIGSTOP cannot be caught or ignored.
void SignalHandler::set_sig_catch(void)
{
	if (signal(SIGABRT, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGALRM, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGBUS, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGFPE, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGHUP, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGILL, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGINT, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	// we do not use pipes...
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGPOLL, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGPROF, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));				

	if (signal(SIGPWR, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGQUIT, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGSEGV, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGTSTP, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGSYS, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGTERM, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGTRAP, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGVTALRM, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGXCPU, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGXFSZ, sig_catch) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		
}

void SignalHandler::set_sig_default(void)
{
	if (signal(SIGABRT, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGALRM, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGBUS, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGFPE, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGHUP, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGILL, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGPIPE, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGPOLL, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGPROF, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));				

	if (signal(SIGPWR, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGSEGV, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGSYS, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGTERM, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGTRAP, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));

	if (signal(SIGVTALRM, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGXCPU, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		

	if (signal(SIGXFSZ, SIG_DFL) == SIG_ERR)
		throw AppException("webserver", "System error: failed to assign signal handler" + ft_code_location(__FILE__, __LINE__));		
}