/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_54_prepare_redirect.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 17:13:26 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Redirections
//
//		HTTP/1.1 301 Moved Permanently\r\n 
//		Location: https://www.newdomain.com/newpage\r\n
//		\r\n
//
void	AppRun::prepare_redirect(Handler *handler_ptr)
{
	HTTP_Request& request = handler_ptr->http_request_;
	HTTP_Response& response = handler_ptr->http_response_;	

	std::string redirection_phrase = response_status_codes_[request.redirection_code_];
	std::string status_line = "HTTP/1." + ft_itoa(request.http_version_) + " " + ft_itoa(request.redirection_code_) + " " + redirection_phrase + RCLF_;

	std::string location_header = "LOCATION: " + request.redirection_path_ + RCLF_;

	response.response_str_ = status_line + location_header + RCLF_;

	handler_ptr->handler_status_ = READY_TO_RESPONSE_;

}

