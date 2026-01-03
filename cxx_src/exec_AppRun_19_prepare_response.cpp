/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_19_prepare_response.cpp                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 19:59:37 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"
#include "wbs_typedef_enum.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <iostream>


void	AppRun::prepare_response(void)
{
	
	t_socketfd_handler_ptr_map::iterator it = app_handlers_prepare_response_.begin();
	t_socketfd_handler_ptr_map::iterator it_dup = it;
	t_socketfd_handler_ptr_map::iterator it_end = app_handlers_prepare_response_.end();


	while(it != it_end)
	{
		++it_dup;
		
		Handler* handler_ptr = it->second;
		HTTP_Request& request = handler_ptr->http_request_;
		HTTP_Response& response = handler_ptr->http_response_;


		//	ERROR_RESPONSE		- overrides all
		//	< any case bellow >	- IMPORTANT: any case bellow can switch to ERROR_RESPONSE and call 'prepare_error' in case of an error
		//		CHUNK			- overrides everything, except ERROR_RESPONSE : that means, we already processed some case, got response and now need to send a chunk of it
		//		CGI				- overrides any method, we need to pass vars to 'cgi_env' and exceve CGI, METHOD - is just a specific way to deliver vars to CGI
		//		METHOD			- if non of above then we resolve method and prepare response accordingly, response maybe chunked - then next loop it goes to CHUNK
		int idx =	(request.method_ == DELETE_) *	RESPONSE_DELETE_ + \
					(request.method_ == GET_) *		RESPONSE_GET_ + \
					(request.method_ == HEAD_) *	RESPONSE_HEAD_ + \
					(request.method_ == POST_) *	RESPONSE_POST_ + \
					(request.method_ == PUT_) *		RESPONSE_PUT_;

		idx -= (idx - 								RESPONSE_CGI_) * (request.is_cgi_ == YES_);	// overrides methods
		idx -= (idx - 								RESPONSE_REDIRECT_) * (request.redirection_code_ != NOT_SET_);	// overrides CGI, methods					
		idx -= (idx - 								RESPONSE_CHUNK_) * (response.is_chunked_);	//  overrides everything, except ERROR
		idx *= (!(handler_ptr->handler_status_ == 	ERROR_RESPONSE_));							//  overrides everything


		switch (idx)
		{
			case RESPONSE_ERR_:
			{
				prepare_error(handler_ptr);
				break;
			}
			case RESPONSE_CHUNK_:
			{
				prepare_chunk(handler_ptr);
				break;
			}
			case RESPONSE_REDIRECT_:
			{
				prepare_redirect(handler_ptr);
				break;
			}						
			case RESPONSE_CGI_:
			{
				prepare_cgi(handler_ptr);
				break;
			}
			case RESPONSE_DELETE_ :
			{
				prepare_delete(handler_ptr);
				break;
			}
			case RESPONSE_GET_ :
			{
				prepare_get(handler_ptr);
				break;
			}
			case RESPONSE_POST_ :
			{
				prepare_post(handler_ptr);
				break;
			}
			default:
			{
				throw AppException(prog_name_, "Algorithm Error: " + ft_code_location(__FILE__, __LINE__));
				break;
			}
		}


		if (handler_ptr->handler_status_ == READY_TO_RESPONSE_)
		{
			add_to_epoll(handler_ptr->socket_fd_, HANDLER_SEND_, (EPOLLOUT | EPOLLHUP));
			
			handler_ptr->set_timeout(TIMEOUT_FOR_SENDING_RESPONSE_);

			handler_ptr->handler_status_ = WAITING_TO_RESPONSE_;

			app_handlers_can_response_.insert(std::pair<int, Handler*>(it->first, it->second));
			app_handlers_prepare_response_.erase(it);

		}
		else if (handler_ptr->handler_status_ == CGI_LAUNCHED_)
		{

			handler_ptr->set_timeout(TIMEOUT_FOR_GETTING_CGI_);
			handler_ptr->handler_status_ = CGI_WAITING_;
		}

		it = it_dup;
	}	


}
