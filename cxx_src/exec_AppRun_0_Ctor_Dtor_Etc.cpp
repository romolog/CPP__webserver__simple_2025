/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_0_Ctor_Dtor_Etc.cpp                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/07/21 15:09:28 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

AppRun::AppRun(t_config_map& app_config, t_socketfd_listener_map& app_listeners, const std::string& prog_name, char** env) :
	app_config_(app_config),	
	app_listeners_(app_listeners),
	prog_name_(prog_name),
	env_(env)
{


	// https://developer.mozilla.org/en-US/docs/Web/HTTP/Reference/Status
	response_status_codes_[100] = "Continue";
	response_status_codes_[101] = "Switching Protocols";
	response_status_codes_[102] = "Processing";
	response_status_codes_[103] = "Early Hints";
	response_status_codes_[200] = "OK";
	response_status_codes_[201] = "Created";
	response_status_codes_[202] = "Accepted";	
	response_status_codes_[203] = "Non-Authoritative Information";
	response_status_codes_[204] = "No Content";
	response_status_codes_[205] = "Reset Content";
	response_status_codes_[206] = "Partial Content";
	response_status_codes_[207] = "Multi-Status";
	response_status_codes_[208] = "Already Reported";
	response_status_codes_[226] = "IM Used";
	response_status_codes_[300] = "Multiple Choices";
	response_status_codes_[301] = "Moved Permanently";
	response_status_codes_[302] = "Found";
	response_status_codes_[303] = "See Other";
	response_status_codes_[304] = "Not Modified";
	response_status_codes_[305] = "Use Proxy";
	response_status_codes_[307] = "Temporary Redirect";
	response_status_codes_[308] = "Permanent Redirect";
	response_status_codes_[400] = "Bad Request";
	response_status_codes_[401] = "Unauthorized";
	response_status_codes_[402] = "Payment Required";
	response_status_codes_[403] = "Forbidden";
	response_status_codes_[404] = "Not Found";
	response_status_codes_[405] = "Method Not Allowed";
	response_status_codes_[406] = "Not Acceptable";
	response_status_codes_[407] = "Proxy Authentication Required";
	response_status_codes_[408] = "Request Timeout";
	response_status_codes_[409] = "Conflict";
	response_status_codes_[410] = "Gone";
	response_status_codes_[411] = "Length Required";
	response_status_codes_[412] = "Precondition Failed";
	response_status_codes_[413] = "Payload Too Large";
	response_status_codes_[414] = "URI Too Long";
	response_status_codes_[415] = "Unsupported Media Type";
	response_status_codes_[416] = "Range Not Satisfiable";
	response_status_codes_[417] = "Expectation Failed";
	response_status_codes_[418] = "I'm a teapot";
	response_status_codes_[421] = "Misdirected Request";
	response_status_codes_[422] = "Unprocessable Entity";
	response_status_codes_[423] = "Locked";
	response_status_codes_[424] = "Failed Dependency";
	response_status_codes_[425] = "Too Early";
	response_status_codes_[426] = "Upgrade Required";
	response_status_codes_[428] = "Precondition Required";
	response_status_codes_[429] = "Too Many Requests";
	response_status_codes_[431] = "Request Header Fields Too Large";
	response_status_codes_[451] = "Unavailable For Legal Reasons";
	response_status_codes_[500] = "Internal Server Error";
	response_status_codes_[501] = "Not Implemented";
	response_status_codes_[502] = "Bad Gateway";
	response_status_codes_[503] = "Service Unavailable";
	response_status_codes_[504] = "Gateway Timeout";
	response_status_codes_[505] = "HTTP Version Not Supported";
	response_status_codes_[506] = "Variant Also Negotiates";
	response_status_codes_[507] = "Insufficient Storage";
	response_status_codes_[508] = "Loop Detected";
	response_status_codes_[510] = "Not Extended";
	response_status_codes_[511] = "Network Authentication Required";
}
