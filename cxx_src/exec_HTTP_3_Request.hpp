/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_HTTP_3_Request.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:40:49 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:51:16 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"
#include "wbs_typedef_enum.hpp"
#include "conf_Location.hpp"

#include <arpa/inet.h> // htons, inet_addr (or inet_pton)
#include <map>
#include <netinet/in.h> // sockaddr_in
#include <sys/socket.h>
#include <string>
#include <vector>

typedef	unsigned long long t_ull;

// hast_tables are used both http_request and cgi_response
//	so we put it to wbs_typedef_enum:
typedef std::map<std::string, std::pair<int, std::string> >		t_hash_table_headers;	// map< field_name , pair< field_encounted_num, field_body > >

// in case of header name multi-presence header values are concatenated with a comma-space ", " delimiter (SECTION 5.2)
// https://www.rfc-editor.org/rfc/rfc9110#name-field-lines-and-combined-fi


class HTTP_Request
{
	public:
		std::string				request_str_;
		std::string::size_type	offset_;	// default 0 : set to the length of each successfully parsed part of the message (request_line, headers, body and optionally trailers)
		int						total_recived_bytes_; // default is 0
	public:
		int						is_parsed_request_line_;	// default 0 = NO_
		int						is_parsed_headers_;			// default 0 = NO_ 
		int						is_parsed_uri_;				// default 0 = NO_
		int						is_parsed_assigned_location_;// default 0 = NO_
		int						is_parsed_header_host_;		// default 0 = NO_
		int						is_parsed_body_;			// default 0 = NO_ 
	public:
		int						method_;	// default is 0 = METHOD_NOT_SET_
		std::string				method_str_;
		std::string				uri_;
		int						http_version_; // default is 1
	public: 	
		std::string				uri_path_;		//	must have, includes target_path and optinally path_info : [scheme :// [user_info @]] /URI_PATH [? query]
		std::string				uri_target_;	//	must have, a part of uri_path_ WITHOUT PATH_INFO;
		std::string				uri_path_info_;	//	maybe empty, a part between target (=CGI_script_name) and QUERY_STR : cgi_path.cgi_ext/CGI_PATH_INFO?query_str
		std::string				uri_hostname_;	//	(empty for relative URI), a part between scheme/user_info and URI_PATH : scheme :// [user_info @] host_name : port /URI_PATH 
		std::string				uri_ext_;		//	maybe empty, first met file extension, to check if it is CGI
		std::string				uri_query_str_;	//	maybe empty, a part between CGI and FRAGMENT : cgi_path.cgi_ext[cgi_path_info]?QUERY_STR#fragement
	public:
		int						redirection_code_; // default -2 NOT_SET_
		std::string				redirection_path_;	// default ""
	public:
		int						is_cgi_; // default 0 = NO_
	public:
		std::string				header_host_hostname_;	// hostname extracted from HEADER 'Host' (obligatory)
		uint16_t				header_host_port_;	// port extracted from HEADER 'Host' (may be empty)		
	public:
		t_hash_table_headers	headers_;
		int						is_chunked_; // default is 0 = NO_
		int						content_length_; // default is -2 = NOT_SET_
		int						is_all_chunks_recieved_; // default is 0 = NO_
		std::string				header_hostname_;
		uint16_t				header_port_;		
	public:
		std::string				body_;
		int						body_size_; // default 0
		int						max_body_; // default NOT_SET_ , if not assigned by config directive, then assigned to REQUEST_MAX_BODY_SIZE_ 
	public:
		int						is_trailers_; // default 0 = NO_
		t_hash_table_headers	trailers_;
	public:
								HTTP_Request				();
};

