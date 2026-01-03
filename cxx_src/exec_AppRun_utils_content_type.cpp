/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_AppRun_utils_content_type.cpp                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 20:26:46 by rponomar          #+#    #+#             */
/*   Updated: 2025/07/21 14:59:46 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec_AppRun.hpp"

#include <iostream>
#include <sstream>


//	HOW TO ASSIGN CONTENT-TYPE
//		https://www.iana.org/assignments/media-types/media-types.xhtml#multipart
//		https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/MIME_types
//		https://www.freeformatter.com/mime-types-list.html

enum FileExt
{
	FEXT_UNKNOWN_ = 0,
	FEXT_BINARY_,
	FEXT_CSS_,
	FEXT_JSON_,
	FEXT_HTML_,
	FEXT_JPEG_,
	FEXT_PDF_,
	FEXT_PNG_,
	FEXT_TAR_,
	FEXT_TXT_,
	FEXT_ZIP_,
	FILE_EXTENSIONS_NUM
};

//	RETURN string "CONTENT-TYPE: <content-type>\r\n"
std::string 	AppRun::set_content_type(const std::string& file_name)
{

	std::string file_ext = "";
	std::string::size_type dot_pos = file_name.find('.');

	// if it is ./relative_path/file.ext
	if (!dot_pos)
		dot_pos = file_name.find('.', 1);

	if (dot_pos != std::string::npos)
		file_ext = file_name.substr(dot_pos);
	
	if (file_ext.empty())
		return ("CONTENT-TYPE: X-whatever/X-whatever\r\n");
		
	//	I support only a few regular content-types: 
	//		text/css					.css
	//		text/html					.html
	//		text/plain					.txt
	//		application/pdf				.pdf
	//		application/x-tar			.tar
	//		application/zip				.zip
	//		image/jpeg					.jpeg .jpg
	//		image/png					.png
	//		application/octet-stream	.bin .dat .exe <binary>
	//	and we use X-type for anything else:
	//		X-whatever/x-whatever		<any other file>

	std::string arr_content_types[FILE_EXTENSIONS_NUM] = 
	{
		"X-whatever/X-whatever",
		"application/octet-stream",
		"text/css",
		"application/json",
		"text/html",
		"image/jpeg",
		"application/pdf",
		"image/png"
		"application/tar",
		"text/plain",
		"application/zip",
	};

	int idx =	FEXT_BINARY_ * (file_ext == ".bin" ||  file_ext == ".dat" ||  file_ext == ".exe") + \
				FEXT_CSS_ * (file_ext == ".css") + \
				FEXT_JSON_ * (file_ext == ".json") + \
				FEXT_HTML_ * (file_ext == ".html") + \
				FEXT_JPEG_ * (file_ext == ".jpg" || file_ext == ".jpeg") + \
				FEXT_PDF_ * (file_ext == ".pdf") + \
				FEXT_PNG_ * (file_ext == ".png") + \
				FEXT_TAR_ * (file_ext == ".tar") + \
				FEXT_TXT_ * (file_ext == ".txt") + \
				FEXT_ZIP_ * (file_ext == ".zip");
	
	return ("CONTENT-TYPE: " + arr_content_types[idx] + RCLF_);
}