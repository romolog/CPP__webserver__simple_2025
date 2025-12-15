/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_6_parsers_cgi.cpp              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:05:28 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"
#include "all_AppException.hpp"

#include <string>
#include <vector>
#include <iostream>
#include <cassert>



void	AppConfig::parse_cgi(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	ServerConfig& new_serv_conf)
{	

	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	//	https://pubs.opengroup.org/onlinepubs/9699919799.2018edition/
	// POSIX : 3.282 Portable Filename Character Set
	// The set of characters from which portable filenames are constructed.
	// A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
	// a b c d e f g h i j k l m n o p q r s t u v w x y z
	// 0 1 2 3 4 5 6 7 8 9 . _ -
	//	IBM recommendation: portable filename length up to 255 chars (so extention should be up to 254 chars)
	std::string cgi_ext(it->content_str_);
	if (	cgi_ext.size() < 2 || \
			cgi_ext.size() > 254 || \
			cgi_ext[0] != '.' || \
			cgi_ext.find_first_not_of(FILE_NAME_RADIX_) != std::string::npos)
		throw AppException(prog_name_, "Conf file is invalid: CGI ext: Size invalid or not Posix portable filename charset: " + it->content_str_);

	new_serv_conf.cgi_ext_ = cgi_ext;

	to_next_no_content_lexem(it, it_end);
}

void	AppConfig::parse_cgi(std::vector<Lexem>::iterator& it, \
	std::vector<Lexem>::iterator& it_end, \
	std::vector<int>& parsed_lexems, \
	Location& new_location)
{	
	++parsed_lexems[it->type_];
	to_next_content_lexem(it, it_end);

	std::string cgi_ext(it->content_str_);
	if (	cgi_ext.size() < 2 || \
			cgi_ext.size() > 254 || \
			cgi_ext[0] != '.' || \
			cgi_ext.find_first_not_of(FILE_NAME_RADIX_) != std::string::npos)
		throw AppException(prog_name_, "Conf file is invalid: CGI ext: Size invalid or not Posix portable filename charset: " + it->content_str_);

	new_location.cgi_ext_ = cgi_ext;

	to_next_no_content_lexem(it, it_end);
}