/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_HTTP_3_Request.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:40:49 by rponomar          #+#    #+#             */
/*   Updated: 2025/07/31 19:23:55 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_HTTP_3_Request.hpp"

HTTP_Request::HTTP_Request() :
	offset_(0),
	total_recived_bytes_(0),
	is_parsed_request_line_(NO_),
	is_parsed_headers_(NO_),		
	is_parsed_uri_(NO_),
	is_parsed_assigned_location_(NO_),
	is_parsed_header_host_(NO_),		
	is_parsed_body_(NO_),
	method_	(METHOD_NOT_SET_),
	http_version_(1),
	redirection_code_(NOT_SET_),
	redirection_path_(""),
	is_cgi_(NO_),
	is_chunked_(NO_),
	content_length_(NOT_SET_),
	is_all_chunks_recieved_(NO_),
	body_size_(0),
	max_body_(NOT_SET_),
	is_trailers_(NO_)
{}


