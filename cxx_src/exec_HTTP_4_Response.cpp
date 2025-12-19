/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_HTTP_4_Response.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 16:40:49 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/17 17:47:51 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_HTTP_4_Response.hpp"

HTTP_Response::HTTP_Response() :
	response_str_("HTTP/1.1 418 I'm a teapot\r\n\r\n"),
	response_code_(418),
	error_(YES_),
	is_chunked_(NO_),
	is_final_chunk_(NO_)
{}


