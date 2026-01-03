/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_09_read_header_trailer_delim.cpp       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/21 19:09:56 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include "all_AppException.hpp"
#include "exec_HTTP_2_Handler.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <cassert>
#include <ctime>
#include <iostream>

//	THIS IS A TOOL FUNCTION

//	used for parsing both headers and trailers from the 'request' string
//	-	parses trailer_name and trailer_body, 
//	-	erases trailer from 'request', 
//	-	save trailer to 'hash_table' (which can be either 'headers_' or 'trailers_')
//	-	does NOT merge trailer to header
//	-	merge bodies for coinsiding names with comma SP (", ") delimiter 
//	parses till the '\r\n\r\n' met
//	noexception (no throw), no exit
//	sets stderr log error message to the '&err_msg' string
//	CHECK: only syntax, does NOT check lexems and logic
//	ERASE: all headers and EMPTY_LINE will be erased from input_str (BODY remains if present)
//	RETURN:
//		 0 : on success (including empty request)
//		-1 : on error, stop executing
//		may leave '&request' string non-empty both on success and on error
int		AppRun::read_headers_trailers(	std::string&			input_str, \
										Handler* 				, \
										std::string&			err_msg, \
										t_hash_table_headers&	hash_table,
										const std::string& 		delimiter)
{
	if (input_str.empty())
		return (0);

	int delim_len = delimiter.size();
	assert(delim_len);

	//	headers and trailers are OPTIONAL, maybe zero of them
	//	https://www.rfc-editor.org/rfc/rfc9112#name-message-format
	//	but there is obligatory empty line, so minimum request-line http-request should still end with "\r\n\r\n"
	std::string::size_type crlf_pos = input_str.find(delimiter);

	//	here we found an empty line (end of header/trailer block)
	std::string::size_type crlf_crlf_pos = input_str.find(delimiter + delimiter);

	if (!crlf_pos || crlf_crlf_pos == std::string::npos)
	{
		err_msg = "<...> " + input_str.substr(0, ERROR_INPUT_SNIPET_LEN_) + " <...>" + ft_code_location(__FILE__, __LINE__);
		return (-1);
	}



	//	sooner or later crlf_pos = 0, because we will reach \r\n\r\n and the last pair \r\n will be at the begining of the line
	while (crlf_pos)
	{
		std::string header = input_str.substr(0, crlf_pos);

		input_str.erase(0, crlf_pos + delim_len);


		crlf_pos = input_str.find(delimiter);

		std::string::size_type colon_pos = header.find(':');

		//	if no colon found or it's a leading one - send ERROR responce
		if (colon_pos == std::string::npos || !colon_pos)
		{
			err_msg = "<...> " + header.substr(0, ERROR_INPUT_SNIPET_LEN_) + " <...>" + ft_code_location(__FILE__, __LINE__);
			return (-1);
		}
			
		//	Each field line consists of a case-insensitive field name followed by a colon 
		//	https://www.rfc-editor.org/rfc/rfc9112#name-field-syntax
		std::string field_name = header.substr(0, colon_pos);
		std::transform(field_name.begin(), field_name.end(), field_name.begin(), ft_toupper);	
		std::string::size_type wrong_name = field_name.find_first_not_of(FIELD_NAME_RADIX_);
		if (wrong_name != std::string::npos)
		{
			err_msg = "<...> " + header.substr(0, ERROR_INPUT_SNIPET_LEN_) + " <...>" + ft_code_location(__FILE__, __LINE__);
			return (-1);
		}

		std::string field_body = header.substr(colon_pos + 1);

		//	folding is supported by HTTP/1.0 and is limited supported by HTTP/1.1 for mediatype message/http
		//	we do not support folding at all
		std::string::size_type is_folded = field_body.find_first_of(delimiter);
		if (is_folded != std::string::npos)
		{
			err_msg = "Folding is not supported" + ft_code_location(__FILE__, __LINE__);
			return (-1);
		}
	

		//	strip optional leading and trailing whitespace
		if (field_body[0] == ' ')
			field_body.erase(0, 1);
		if (field_body[field_body.size() - 1] == ' ')
			field_body.erase(field_body.size() - 1, 1);

		//	finally add to the hash tables of headers
		//
		//	When a field name is repeated within a section, its combined field value consists of 
		//	the list of corresponding field line values within that section, concatenated in order, 
		//	with each field line value separated by a comma SP (", ").
		if (!field_body.empty())
		{
			if (!hash_table.count(field_name))
			{
				std::pair<int, std::string> hash_val(1, field_body);
				hash_table.insert(std::pair< std::string, std::pair<int, std::string> >(field_name, hash_val));
			}
			else
			{
				// increment field counter - it will allow to check if the field is unique (especially for CGI)
				hash_table[field_name].first++;
				// add to 'combined value' of the field
				hash_table[field_name].second +=  ", " + (field_body);
			}
		}

	}
	
	//	erase the last '\r\n' of empty line, so if there is body -
	input_str.erase(0, delim_len);
	
	return (0);
}