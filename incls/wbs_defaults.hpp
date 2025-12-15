/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wbs_defaults.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:45:18 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:48:26 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#define YES_ 1
#define NO_ 0
#define NOT_SET_ -2

#define DEFAULT_CONFIG_FILE_ "./confs/default.conf"

#define ERROR_INPUT_SNIPET_LEN_ 20

#define DIGIT_RADIX_ "0123456789"
#define HEX_RADIX_ "0123456789abcdefABCDEF"

#define DEFAULT_AUTOINDEX_ 1 // "on"
// #define DEFAULT_CLIENT_BUFF_SIZE_ 1024
// #define DEFAULT_CLIENT_BODY_SIZE_ 4096
#define DEFAULT_SERVER_NAME_ ""
#define DEFAULT_INDEX_PAGE_ "index.html"

//127.0.0.1
#define DEFAULT_HOST_ 2130706433
#define DEFAULT_PORT_ 8080

// listen(socket_fd, backlog);
#define DEFAULT_BACKLOG_ 100

//	epoll_wait(..., events_max_num, ...)
//	events_max_num <= DEFAULT_EPOLL_EVENTS_MAX_NUM_
#define DEFAULT_EPOLL_EVENTS_MAX_NUM_ 1024

//	epoll timeout in miliseconds
#define EPOLL_WAIT_TIMEOUT_ 1000

//	how much time in miliseconds is allowed: since connection accepted and before the first chunk of request is received
#define TIMEOUT_FOR_GETTING_REQUEST_ (EPOLL_WAIT_TIMEOUT_ * 10)

//	how much time in miliseconds is allowed: since first chunk received and before request is received completely
#define TIMEOUT_FOR_READING_REQUEST_ (EPOLL_WAIT_TIMEOUT_ * 20)

//	ToDo replace by TIMEOUT_FOR_SENDING_RESPONSE_
//	how much time in miliseconds is allowed: since set to ERROR_RESPONCE_ and before send responce to the client
#define TIMEOUT_FOR_ERROR_RESPONSE_ (EPOLL_WAIT_TIMEOUT_ * 20)

//	how much time in miliseconds is allowed: since CGI launched and before the full CGI output is received
#define TIMEOUT_FOR_GETTING_CGI_ (EPOLL_WAIT_TIMEOUT_ * 20)

//	how much time in miliseconds is allowed: since RESPONSE_STR is ready and before the LAST chunk of response is sent
#define TIMEOUT_FOR_SENDING_RESPONSE_ (EPOLL_WAIT_TIMEOUT_ * 30)

//	everage request size is between 800B and 2KB
// https://en.wikipedia.org/wiki/Query_string
//	Various ad hoc limitations on request-line length are found in practice. It is RECOMMENDED that all HTTP senders and recipients support, at a minimum, request-line lengths of 8000 octets.[13]
#define SERVER_BUFFER_SIZE_ 8192
// #define SERVER_BUFFER_SIZE_ 512
#define REQUEST_MAX_BODY_SIZE_ 16384
#define REQUEST_MAX_BODY_SIZE_STR_ "16384"

//	maximum allowed size of chunked message in bytes
#define MAX_CHUNKED_MSG_SIZE (SERVER_BUFFER_SIZE_ * 10)

#define RCLF_ "\r\n"
#define RCLF_RCLF_ "\r\n\r\n"
#define FIELD_NAME_RADIX_ "!\"#$%&\'()*+'-./0123456789;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
#define FILE_NAME_RADIX_ "-.0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"
#define PATH_NAME_RADIX_ "-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ_abcdefghijklmnopqrstuvwxyz"

//	Hostnames are composed of a sequence of labels concatenated with dots. For example, "en.wikipedia.org" is a hostname. 
//	Each label must be 1 to 63 octets long.[2] The entire hostname, including the delimiting dots, has a maximum of 253 ASCII characters.[4]
//	The Internet standards (Request for Comments) for protocols specify that labels may contain only the ASCII letters a through z (in a case-insensitive manner), 
//	the digits 0 through 9, and the hyphen-minus character ('-'). The original specification of hostnames required that labels start with an alpha character and not end with a hyphen.[5] 
//	However, a subsequent specification permitted hostname labels to start with digits.
#define HOSTNAME_RADIX_ "-./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"


#define CONTENT_LEN_NOT_SET_ (-2)
#define CONTENT_LEN_ERROR_ (-1)

#define URI_MAX_LEN_ 1024

#define HTML_START_ "	<!DOCTYPE html>\n 																								\
						<html lang=\"en\">\n																							\
						<head>\n 																										\
							<meta charset=\"utf-8\">\n																					\
							<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">													\
							<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">									\
							<meta name=\"description\" content=\"42 webserver RPONOMAR IKHANENK\">										\
							<meta name=\"author\" content=\"RPONOMAR IKHANENK\">														\
																																		\
							<title>42 webserver - fully static website</title>															\
																																		\
							<link href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.6/css/bootstrap.min.css\" rel=\"stylesheet\">	\
																																		\
						</head>																											\
						<body>																											\
							<div class=\"container\">																					\
							<header style=\"background-color: #7777FF\">																\
								<h1>WEBSERV 42</h1>																			\
							</header>																									\
					"

#define	ERROR_PAGE_DEFAULT_ "./website_42/error_default.html"
