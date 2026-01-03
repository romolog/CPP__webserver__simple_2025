/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conf_AppConfig.hpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 17:17:31 by rponomar          #+#    #+#             */
/*   Updated: 2025/06/22 22:13:27 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "wbs_defaults.hpp"
#include "wbs_err_messages.hpp"
#include "wbs_shell_colors.hpp"
#include "wbs_typedef_enum.hpp"

#include "all_utils.hpp"

#include "conf_Lexem.hpp"
#include "conf_ServerConfig.hpp"

#include <limits>
#include <map>
#include <set>
#include <string>
#include <sys/stat.h>
#include <vector>

typedef std::map< uint64_t, ServerConfig >		t_config_map;

class AppConfig
{
//	RESULT:
	//			app_config_:	map of virtual server configs: KEY = pair(host, port), VALUE = (server_name, server_parameters)
	//							originally config_file could have many server_names per one server, but we split them and just copy ServerConfig to each server_name_
	//							designed for 2 stage of server resolving:
	//								1) find matching host-port - results into subset of values, if not found - send error-response
	//								2) find matching server_name, if not found - use default server for this host-port
	public:
		t_config_map				app_config_;
		std::set<uint64_t>			app_host_ports_;		

	public:
		t_config_map				make_app_config							(void);
		std::set<uint64_t>			get_app_host_ports						(void);

//	INPUT:
	//		conf_file: a name of configuration file: default or argv[1]
	//		prog_name: a name of prog: argv[0] after some cosmetic processing
	private:
		const std::string&			conf_file_;
		const std::string&			prog_name_;


//	TOOLS ------------------------------------------------------			
//		used for processing config file into 'resulting member'
	private:
		std::string					conf_str_;
		std::vector<std::string>	tokens_;
		std::vector<Lexem>			lexems_;

	private:
		typedef void (AppConfig::*	ServerLexemParserPtr)(std::vector<Lexem>::iterator&, std::vector<Lexem>::iterator&, std::vector<int>&, ServerConfig&);	
		typedef void (AppConfig::*	LocationLexemParserPtr)(std::vector<Lexem>::iterator&, std::vector<Lexem>::iterator&, std::vector<int>&, Location&);

//	CONFIG FILE to CONFIG STR to LEXEMS to CONFIG_PAIR		
	private:
		void						is_valid_file							(void);
		void						file_to_string							(void);
		void						eliminate_comments						(void);		
		void						string_to_tokens						(void);
		void						tokens_to_lexems						(void);
		void						lexems_to_config						(void);

//	CONFIG PARSING		
	private:
		t_config_map				parse_server_config						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end);
	private:	
		void						throw_exception							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						throw_exception							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);

	private:
		void						parse_autoindex							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_autoindex							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);

		void						parse_brace								(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_lexems, int brace_type);

		void						parse_brace_open						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_brace_open						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);

		void						parse_brace_close						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_brace_close						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);

		void						parse_cgi								(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_cgi								(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);

		void						parse_cgi_rewrite						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_cgi_rewrite						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);

		void						parse_client_max_body					(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_client_max_body					(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);

		void						parse_error_page						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_error_page						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);

		void						parse_index_page						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_index_page						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);		

		void						parse_listen							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_listen							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);		
		u_int32_t					get_host								(std::string::size_type host_end, std::vector<Lexem>::iterator& it);
		u_int16_t					get_port								(std::string& port_str, std::vector<Lexem>::iterator& it);

		void						parse_location							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_location							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);
		std::string&				get_location_path						(std::string& path_str);

		void						parse_limit_except_method				(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_limit_except_method				(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);		

		void						parse_redirections						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_redirections						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);		

		void						parse_root								(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_root								(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);		

		void						parse_server_name						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_server_name						(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);		

		void						parse_upload							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_server_lexems, ServerConfig& new_serv_conf);
		void						parse_upload							(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end, std::vector<int>& parsed_location_lexems, Location& new_location);		

//	CONFIG PARSING UTILS		
	private:
		// void						reset_tool_variables					(void);
		void						count_parsed_unique_lexem				(int lexem_type, std::vector<int>& parsed_lexems, std::vector<Lexem>::iterator& it);
		int							to_next_content_lexem					(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end);
		int							to_next_no_content_lexem				(std::vector<Lexem>::iterator& it, std::vector<Lexem>::iterator& it_end);

//	CONFIG SET DEFAULTS, CHECK OBLIGATORY and REDANDANT fields

	private:
		void						set_server_defaults						(std::vector<int>& parsed_lexems, ServerConfig& new_serv_conf);
		void						set_location_defaults					(std::vector<int>& parsed_lexems, Location& new_location);

		void						check_server_obligatories				(std::vector<int>& parsed_lexems);
		// void						check_location_obligatories				(std::vector<int>& parsed_lexems);

		void						check_server_redundants					(std::vector<int>& parsed_lexems);
		void						check_location_redundants				(std::vector<int>& parsed_lexems);

		// void						check_server_noncompatible_fields		(std::vector<int>& parsed_lexems);
		// void						check_location_noncompatible_fields		(std::vector<int>& parsed_lexems);
		

//	GENERATE CONFIG MAP for each parsed 'server{...}'
	private:
		t_config_map				generate_config_map						(ServerConfig& new_serv_conf);		


//	CDTORS ------------------------------------------------------
	public:
									AppConfig								(const std::string& conf_file_name, const std::string& prog_name);		
};
	
