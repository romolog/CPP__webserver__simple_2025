/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_52_prepare_error.cpp                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 17:14:39 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <dirent.h>
#include <fstream>
#include <iostream>
#include <sstream>


//	Throws exception on error
//	sets error_body ot default error-page with correct error-code and error-phrase
void	AppRun::default_error_body(const int error_code, const std::string& error_page_path, std::string& error_body)
{
	std::string err_msg;
	int check = ft_file_to_str(error_page_path, error_body, err_msg);
	if (check == -1)
		throw AppException(prog_name_, "Can't assign default error body: Default error page failed: " + err_msg + ft_code_location(__FILE__, __LINE__));

	// place error code into error-page
	std::string error_code_str = ft_itoa(error_code);
	std::string::size_type code_pos = error_body.find("__ERROR_CODE__18__");
	if (code_pos == std::string::npos)
		throw AppException(prog_name_, "Can't assign default error body: Default error page failed: " + err_msg + ft_code_location(__FILE__, __LINE__));		
	error_body.replace(code_pos, 18, error_code_str);

	//	place error phrase
	std::string error_phrase = response_status_codes_[error_code];
	std::string::size_type phrase_pos = error_body.find("__ERROR_PHRASE__20__");
	if (phrase_pos == std::string::npos)
		throw AppException(prog_name_, "Can't assign default error body: Default error page failed: " + err_msg + ft_code_location(__FILE__, __LINE__));	
	error_body.replace(phrase_pos, 20, error_phrase);

}


void	AppRun::prepare_error(Handler *handler_ptr)
{
	HTTP_Request& request = handler_ptr->http_request_;
	HTTP_Response& response = handler_ptr->http_response_;	
	ServerConfig& serv_config = handler_ptr->config_server_;
	Location& location = handler_ptr->config_location_;

	int error_code = response.response_code_;
	std::string error_phrase = response_status_codes_[error_code];

	std::string& error_body = response.full_body_;
	std::string content_type = "";
		

	std::map<int, std::string>* err_pages = &location.err_pages_;

	if (err_pages->empty())
		err_pages = &serv_config.err_pages_;

	if (!err_pages->empty() && err_pages->count(error_code))
	{

		// check if error_page is valid
		std::string& error_page_path = (*err_pages)[error_code];
		std::string err_msg;
		int check = ft_file_to_str(error_page_path, error_body, err_msg);
		if (check == -1)
		{
			std::cerr << TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ << err_msg << ft_code_location(__FILE__, __LINE__) << '\n';
			default_error_body(error_code, ERROR_PAGE_DEFAULT_, error_body);
			content_type = "CONTENT-TYPE: text/html\r\n";
		}
		else
			content_type = set_content_type(error_page_path);
	}
	else
	{
		// ERROR PAGES EMPTY OR NOT FOR OUR CODE
		
		default_error_body(error_code, ERROR_PAGE_DEFAULT_, error_body);
		content_type = "CONTENT-TYPE: text/html\r\n";
	}
	
	std::string status_line = "HTTP/1." + ft_itoa(request.http_version_) + " " + ft_itoa(error_code) + " " + error_phrase + RCLF_;	

	assemble_chunk_or_unchunk(handler_ptr, response, status_line, content_type);
	handler_ptr->handler_status_ = READY_TO_RESPONSE_;

}

