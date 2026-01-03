/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_24_prepare_cgi.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:27:58 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string.h>
#include <sys/wait.h>

//	IMPORTANT: CGI SPECIFICATIONS BY RFC:
//		https://www.rfc-editor.org/rfc/rfc3875

//	IMPORTANT: README:
//		https://www.garshol.priv.no/download/text/http-tut.html

//	CGI HOW TO PASS PARAMETERS:
//		https://en.wikipedia.org/wiki/Common_Gateway_Interface
//		The CGI specification defines how additional information passed with the request is passed to the script. 
//		The Web server creates a subset of the environment variables passed to it and adds details pertinent to the HTTP environment. 
//		For instance, if a slash and additional directory name(s) are appended to the URL immediately after the name of the script (in this example, /with/additional/path), 
//		then that path is stored in the PATH_INFO environment variable before the script is called. 
//		If parameters are sent to the script via an HTTP GET request (a question mark appended to the URL, 
//		followed by param=value pairs; in the example, ?and=a&query=string), then those parameters are stored in the QUERY_STRING environment variable before the script is called. 
//		Request HTTP message body, such as form parameters sent via an HTTP POST request, are passed to the script's standard input. 
//		The script can then read these environment variables or data from standard input and adapt to the Web browser's request.[8]
//
//	CGI HOW TO PASS PARAMETERS: IMPORTAT: README
//		https://www.rfc-editor.org/rfc/rfc9110#name-application-handling-of-fie


//		launch of CGI process, the result will be catched at later step by cgi_catch()
//		launch and catch are splited for the purpose of parallel execution of all CGI processes
void	AppRun::prepare_cgi(Handler *handler_ptr)
{
	
	if (handler_ptr->handler_status_ == REQUEST_RECIEVED_)
	{
		cgi_launch(handler_ptr);
		cgi_catch(handler_ptr);
	}
	else if (handler_ptr->handler_status_ == CGI_LAUNCHED_ || handler_ptr->handler_status_ == CGI_WAITING_)
	{
		cgi_catch(handler_ptr);
	}
	else
	{
		//	CGI_CATCHED_		
		process_cgi_response(handler_ptr);
	}
}
