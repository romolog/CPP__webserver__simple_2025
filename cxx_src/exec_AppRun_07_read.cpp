/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_07_read.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 18:21:12 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <ctime>
#include <iostream>
#include <ostream>

#define FAIL_ -1
#define READ_SUCCESS_ 1
#define QUIT_ 1

//	HTTP/1.0 or HTTP/1.1 : 8 charachters
#define HTTP_VERSION_STR_LEN_ 8

//	Max length of method name: 'delete' is the longest one with length 6
#define METHOD_MAX_LEN_ 6

	//	MAIN DOCUMENTS:
	//
	//		https://www.rfc-editor.org/rfc/rfc822
	//			LWSP-char   =  SPACE / HTAB 
	//			CRLF        =  CR LF
	//			CR          =  <ASCII CR, carriage return>  ; (     15,      13.)
	//			LF          =  <ASCII LF, linefeed>         ; (     12,      10.)
	//			
	//			field_name:field_body CRLF
	//
	//			       Full-Request   = Request-Line             ; Section 5.1
	//									*( General-Header        ; Section 4.3
	//									| Request-Header        ; Section 5.2
	//									| Entity-Header )       ; Section 7.1
	//									CRLF
	//									[ Entity-Body ]          ; Section 7.2
	//									CRLFCRLF




	//	MOZILA DOCUMENTS:
	//	https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Messages

	//	NODE.JS DOCUMENTS:
	//	https://nodejs.org/api/http.html

	//	READING:
	//		- AppRun reads requests and set up all HTTP_Clients fields
	//		- AppRun handles errors and switches to other event handling

	//	READING:
	//		-	can client read at the moment? (maybe client should first reply previous request)
	//		-	set TIMEOUT to receive a complete request: start_time and duration, if not set already
	//		-	read to buffer:
	//				-	if message size is equal to buffer - send ERROR-RESPONSE: 400 BAD REQUEST
	//				-	if 0 read: client closed at its end, so we also close connection
	//				??? -	if -1 read: system error, we close connection, SHOULD WE TRY TO SEND ERROR_MESSAGE : 500 INTERNAL SERVER ERROR
	//		-	check if total message volume is within limits (count total recived bytes)
	//					-	send BAD REQUEST responce if beyond limit
	//		-	parse REQUEST-LINE of the request (doesn't matter if it is chunk or completed)
	//				Request-Line = Method SP Request-URI SP HTTP-Version CRLF
	//					- on invalid input switch to ERROR-RESPONSE: 400 BAD REQUEST
	//					- otherwise save METHOD, URI
	//		-	parse HEADERS
	//				field-line   = field-name ":" OWS field-value OWS
	//				https://www.rfc-editor.org/rfc/rfc9112#name-field-syntax
	//				Each field line consists of a case-insensitive field name followed by a colon (":"), optional leading whitespace, the field line value, and optional trailing whitespace.
	//					- on invalid input switch to ERROR-RESPONSE: 400 BAD REQUEST
	//					- otherwise save HEADERS to hash-table
	//		-	get CHUNK of request (request is not yet completed):
	//					save received chunk to ???
	//					set status to READING_IN_PROGRESS_
	//					quit from reading till the next chunk (do not do anything else)
	//		-	get COMPLETED requerst: 
	//					check if request is completed 
	//		-	get HEADER

//	RETURN:	 0	if client can't read at the moment
//			 N	number of read bytes: if client can read and read was successful
//			-1	if 
int		AppRun::read_request(const int client_socket_fd)
{	

	Handler* handler_ptr = app_handlers_can_read_[client_socket_fd];

	//	set timeout: if during timout duration COMPLETE request is not recieved, we close this client and connection socket	
	if (handler_ptr->handler_status_ == READY_TO_READ_)
	{
		handler_ptr->set_timeout(TIMEOUT_FOR_READING_REQUEST_);
		handler_ptr->handler_status_ = READING_IN_PROGRESS_;
	}
	
	// set buffer for reading request (if message exceeds our buffer it is an error 400 Bad request);
	std::vector<char> buff_vector;
	buff_vector.reserve(SERVER_BUFFER_SIZE_ + 1);
	char* buffer = buff_vector.data();
	// char buffer[SERVER_BUFFER_SIZE_ + 1];
	int check_read = read_from_socket_to_buffer(client_socket_fd, handler_ptr, buffer);
	if (check_read <= 0)
		return (check_read);

	handler_ptr->http_request_.total_recived_bytes_ += check_read;
	if (handler_ptr->http_request_.total_recived_bytes_ > MAX_CHUNKED_MSG_SIZE)
	{
		// Payload Too Large 413
		set_error_response(handler_ptr, "Total received request message is too large", 413);
		switch_to_prepare_response(handler_ptr);
		return (QUIT_);
	}


	//	TCP send fragments of message, sometimes very short (20-30 chars)
	//	we need to accumulate them to get a complete request
	//
	handler_ptr->http_request_.request_str_ += buffer;

	//	parse request line:
	int check_request_line = parse_request_line(handler_ptr, handler_ptr->http_request_.request_str_);

	
	//	IF -1(error) or 1(not yet recieved input enough) is returned, then QUIT to 05_HANDLE_EVENT.cpp or KILL_TIMEOUT or PREPARE_RESPONSE
	if (check_request_line)
	{
		return (QUIT_);
	}

	int check_headers = parse_headers(client_socket_fd, handler_ptr, handler_ptr->http_request_.request_str_);
	if (check_headers)
		return (QUIT_);
	
	int check_read_uri = parse_uri(handler_ptr, handler_ptr->http_request_.uri_);
	if (check_read_uri)
		return (QUIT_);


	int check_host = parse_header_host(handler_ptr);
	if (check_host)
		return (QUIT_);


	int	check_assign = assign_location_to_request(handler_ptr);
	if (check_assign)
		return (QUIT_);		
	

	int check_body = parse_body(handler_ptr, handler_ptr->http_request_.request_str_);
	if (check_body)
		return (QUIT_);

	//	trailers can't include necessary info, only supplementary
	//	we can discard them
	//	we decided to discard them all and always
	
	//	SWITCH TO RESPONCE
	handler_ptr->handler_status_ = REQUEST_RECIEVED_;
	app_handlers_can_read_.erase(client_socket_fd);
	delete_from_epoll(client_socket_fd);
	app_handlers_prepare_response_.insert(std::pair<int, Handler*>(client_socket_fd, handler_ptr));
	
	return (READ_SUCCESS_);

}

int		AppRun::read_from_socket_to_buffer(const int client_socket_fd, Handler* handler_ptr, char* buffer)
{
	void* buffer_ptr = static_cast<void *>(buffer);

	//	ssize_t recv(int socket_fd, void *buffer, size_t len_of_buffer, int flags);
	//		don't use flag, no behavior modification for recv
	//	we limit allowed message size to our buffer_size, based on SECTION 2.3 https://www.rfc-editor.org/rfc/rfc9110#name-framing-and-completeness
	ssize_t check_recv = recv(client_socket_fd, buffer_ptr, SERVER_BUFFER_SIZE_, 0);
	if (check_recv <= 0)
	{
		if (check_recv == -1)
			std::cerr	<< TAB_ << prog_name_ << RED_BOLD_HIGH_ << ERROR_ << DEFAULT_COLOR_ \
						<< "System error: Can't recv to buffer from socket " << client_socket_fd << '\n';
		return (check_recv);

	}
	else if (check_recv == SERVER_BUFFER_SIZE_)
	{
		set_error_response(handler_ptr, "Request is too large: Buffer is full", 413);
		return (-1);
	}

	buffer[check_recv] = '\0';

	return (check_recv);
}
