/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wbs_typedef_enum.hpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 19:45:18 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 16:37:12 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <string>

typedef	unsigned long long t_ull;
typedef std::map<std::string, std::pair<int, std::string> >		t_hash_table_headers;	// map< field_name , pair< field_encounted_num, field_body > >

enum ResponceTypes
{
	RESPONSE_ERR_ = 0,
	RESPONSE_CHUNK_,	
	RESPONSE_REDIRECT_,
	RESPONSE_CGI_,
	RESPONSE_DELETE_,
	RESPONSE_GET_,
	RESPONSE_POST_,
	RESPONSE_TYPES_NUM
};

enum HandlerFunctions
{
	IGNORE_F_,
	ACCEPT_F_,
	RECV_F_,
	SEND_F_,
	CLOSE_F_,
	NUM_HANDLER_FUNCTIONS
};

enum HandlerStatuses
{
	HANDLER_STATUS_NOT_SET_,
	READY_TO_READ_,
	READING_IN_PROGRESS_,
	REQUEST_RECIEVED_,
	CGI_LAUNCHED_,
	CGI_WAITING_,
	CGI_CATCHED_,
	READY_TO_RESPONSE_,
	WAITING_TO_RESPONSE_,	
	ERROR_RESPONSE_,
	TO_CLOSE_,
	NUM_Handler_STATUSES
};

#define HANDLER_TYPES(HT)	\
	HT(HANDLER_NOT_SET_),	\
	HT(HANDLER_LISTEN_),	\
	HT(HANDLER_RECV_),		\
	HT(HANDLER_SEND_)

#define HT(f) f
	enum HandlerTypes {HANDLER_TYPES(HT), NUM_HANDLER_TYPES_};
#undef HT

enum HTTP_MethodTypes
{
	METHOD_NOT_SET_ = 0,
	DELETE_,
	GET_,
	POST_,
	NUM_HTTP_METHOD_TYPES
};