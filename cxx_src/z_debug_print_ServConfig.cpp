/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   z_debug_print_ServConfig.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/15 17:48:53 by rponomar          #+#    #+#             */
/*   Updated: 2025/07/23 16:34:05 by rponomar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "conf_ServerConfig.hpp"

#include <iostream>

// void	print_location(Location& location)
// {
// 	std::cout << "\t\t\tpath_: " << location.path_ << '\n' << \
// 			"\t\t\tautoindex: " << location.autoindex_ << '\n';
			
// 	{
// 		std::map<std::string, std::string>::iterator it = location.cgis_.begin();
// 		std::map<std::string, std::string>::iterator it_end = location.cgis_.end();
// 		std::cout << "\t\t\tcgis_:\n";
// 		for(; it != it_end; ++it)
// 			std::cout << "\t\t\text: " << it->first << " : cgi: " << it->second << '\n';
// 	}

// 	std::cout << "\t\t\tclient_body_buff_size_: " << location.client_body_buff_size_ << '\n' << \
// 				"\t\t\tclient_max_body_size_: " << location.client_max_body_size_ << '\n';
	
// 	{
// 		std::map<int, std::string>::iterator it = location.err_pages_.begin();
// 		std::map<int, std::string>::iterator it_end = location.err_pages_.end();
// 		std::cout << "\t\t\terr_pages_:\n";
// 		for(; it != it_end; ++it)
// 			std::cout << "\t\t\t\terr: " << it->first << " : page: " << it->second << '\n';
// 	}	

// 	std::cout << "\t\t\tindex_page_: " << location.index_page_ << '\n';

// 	{
// 		std::map<std::string, std::string>::iterator it = location.redirections_.begin();
// 		std::map<std::string, std::string>::iterator it_end = location.redirections_.end();
// 		std::cout << "\t\t\tredirections_:\n";
// 		for(; it != it_end; ++it)
// 			std::cout << "\t\t\t\told: " << it->first << " : new: " << it->second << '\n';
// 	}

// 	std::cout << "\t\t\troot_: " << location.root_ << '\n' << \
// 	"\t\t\tupload_dir_: " << location.upload_dir_ << '\n';
// }

// void	print_serv_config(ServerConfig& serv_conf)
// {
// 	std::cout << "\n\t------------------------------\n\t\tSERVER_CONFIG:\n\n" << \
// 			"\tserver_name: " << *(serv_conf.server_names_.begin()) << '\n'; 
			
// 	{
// 		std::multimap<uint32_t, uint16_t>::iterator it = serv_conf.host_ports_.begin();
// 		std::multimap<uint32_t, uint16_t>::iterator it_end = serv_conf.host_ports_.end();
// 		std::cout << "\thost_ports:\n";
// 		for(; it != it_end; ++it)
// 			std::cout << "\t\thost: " << it->first << " : port: " << it->second << '\n';
// 	}
	
// 	{
// 		std::vector<Location>::iterator it = serv_conf.locations_.begin();
// 		std::vector<Location>::iterator it_end = serv_conf.locations_.end();
// 		std::cout << "\tLOCATIONS:\n";
// 		for(int i = 0; it != it_end; ++it, ++i)
// 		{
// 			std::cout << "\t\t# " << i << ": Location: \n"; 
// 			print_location(*it);
// 		}
// 	}	

// }