/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig_2_file_string_tokens_lexems.cp :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 20:59:00 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:01:48 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_AppConfig.hpp"

#include "all_AppException.hpp"
#include "all_utils.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#define BLANK_RADIX_ " \f\r\t\v"
#define COMMENT_RADIX_ "#;"
#define MIN_TOKENS_NUM_ 11
#define SPACE_RADIX_ " \f\n\r\t\v"

#define REQUIRED_LEXEMS_ 3

void	AppConfig::is_valid_file(void)
{
	if (conf_file_.empty())
		throw AppException(prog_name_, "Conf file name is empty: " + conf_file_);

	struct stat file_info;
	if (stat(conf_file_.c_str(), &file_info))
		throw AppException(prog_name_, "Can't access conf file: " + conf_file_); 
	else if (file_info.st_mode & S_IFREG)
			return ;
	else if (file_info.st_mode & S_IFDIR)
		throw AppException(prog_name_, "Conf file is a directory: " + conf_file_);
	else
		throw AppException(prog_name_, "Conf file isn't a regular file: " + conf_file_);
}

void	AppConfig::file_to_string(void)
{
	std::ifstream ifs(conf_file_.c_str());
	if (!ifs.is_open())
		throw AppException(prog_name_, "Fail to open conf file: " + conf_file_);

	std::stringstream ss;
	ss << ifs.rdbuf();
	if (!ifs.good())
		throw AppException(prog_name_, "Fail to read conf file: " + conf_file_);
	conf_str_ = ss.str();
	
	if (conf_str_.empty())
	throw AppException(prog_name_, "Conf file is empty: " + conf_file_);

}

void	AppConfig::eliminate_comments(void)
{
	std::string::size_type comment_start = conf_str_.find_first_of(COMMENT_RADIX_);
	std::string::size_type comment_end = conf_str_.find('\n', comment_start);
	if (comment_end == std::string::npos)
		comment_end = conf_str_.size();
	
	std::string::size_type comment_len = comment_end - comment_start;
	
	while (comment_start != std::string::npos)
	{
		conf_str_.replace(comment_start, comment_len, " ");

		comment_start = conf_str_.find_first_of(COMMENT_RADIX_);
		comment_end = conf_str_.find('\n', comment_start);
		if (comment_end == std::string::npos)
			comment_end = conf_str_.size();
			
		comment_len = comment_end - comment_start;

	}

	if (conf_str_.find_first_not_of(SPACE_RADIX_) == std::string::npos)
		throw AppException(prog_name_, "Conf file has only comments: " + conf_file_ + ft_code_location(__FILE__, __LINE__));

}


void	AppConfig::string_to_tokens(void)
{
	std::string::size_type token_start = conf_str_.find_first_not_of(SPACE_RADIX_);
	std::string::size_type token_end = conf_str_.find_first_of(SPACE_RADIX_, token_start);
	std::string::size_type token_len = token_end - token_start;
	while (token_start != std::string::npos)
	{
		tokens_.push_back(conf_str_.substr(token_start, token_len));
		token_start = conf_str_.find_first_not_of(SPACE_RADIX_, token_end);
		token_end = conf_str_.find_first_of(SPACE_RADIX_, token_start);
		token_len = token_end - token_start;
	}

	if (tokens_.empty())
		throw AppException(prog_name_, "Conf file is space: " + conf_file_);
	else if (tokens_.size() < MIN_TOKENS_NUM_)
		throw AppException(prog_name_, "Conf file lacks info: " + conf_file_);
}

void	AppConfig::tokens_to_lexems(void)
{
	std::vector<std::string>::iterator it = tokens_.begin();
	std::vector<std::string>::iterator it_end = tokens_.end();
	
	for (; it != it_end; ++it)
	{
		int lex_type = LXM_A_CONTENT_;

		lex_type += \
					LXM_AUTOINDEX_ * (*it == "autoindex") + \
					LXM_BRACE_OPEN_ * (*it == "{") + \
					LXM_BRACE_CLOSE_ * (*it == "}") + \
					LXM_CGI_ * (*it == "cgi") + \
					LXM_CGI_REWRITE_ * (*it == "cgi_rewrite") + \
					LXM_CLIENT_MAX_BODY_ * (*it == "client_max_body_size") + \
					LXM_ERROR_PAGE_ * (*it == "error_page") + \
					LXM_INDEX_PAGE_ * (*it == "index_page") + \
					LXM_LIMIT_EXCEPT_ * (*it == "limit_except") + \
					LXM_LISTEN_ * (*it == "listen") + \
					LXM_LOCATION_ * (*it == "location") + \
					LXM_RETURN_ * (*it == "return") + \
					LXM_ROOT_ * (*it == "root") + \
					LXM_SERVER_NAME_ * (*it == "server_name") + \
					LXM_UPLOAD_ * (*it == "upload") + \
					LXM_Z_SERVER_ * (*it == "server");

		Lexem lexem(lex_type, *it);
		lexems_.push_back(lexem);
	}
}
