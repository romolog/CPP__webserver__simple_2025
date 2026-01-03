/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_Lexem.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/10 16:29:02 by rponomar          #+#    #+#             */
/*   Updated: 2025/09/18 19:45:12 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stdint.h>
#include <string>

// CONTENT is intentionally the first, SERVER is intentionally the last
#define LEXEM_TYPES(LT) 				\
		LT(LXM_A_CONTENT_),					\
	LT(LXM_AUTOINDEX_),						\
	LT(LXM_BRACE_OPEN_),					\
	LT(LXM_BRACE_CLOSE_),					\
	LT(LXM_CGI_),							\
	LT(LXM_CGI_REWRITE_),					\
	LT(LXM_CLIENT_MAX_BODY_),				\
	LT(LXM_ERROR_PAGE_),					\
	LT(LXM_INDEX_PAGE_),					\
	LT(LXM_LIMIT_EXCEPT_),					\
	LT(LXM_LISTEN_),						\
	LT(LXM_LOCATION_),						\
	LT(LXM_RETURN_),						\
	LT(LXM_ROOT_),							\
	LT(LXM_SERVER_NAME_),					\
	LT(LXM_UPLOAD_),						\
		LT(LXM_Z_SERVER_)

#define LT(f) f
	enum LexemType {LEXEM_TYPES(LT), NUM_LEX_TYPES_};
#undef LT

struct Lexem
{
	public:
		int				type_;
		std::string		content_str_;
	public:
		Lexem(int type, std::string& content_str);	
};
