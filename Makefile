# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: rponomar <rponomar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/05/30 20:25:38 by rponomar          #+#    #+#              #
#    Updated: 2025/09/21 19:54:19 by rponomar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

ifeq ($(origin CXX), default)
CXX = c++
endif

ifeq ($(origin RM), default)
RM = rm -rf
endif

CXXSRC := \
	./cxx_src/main.cpp											\
	./cxx_src/all_AppException.cpp								\
	./cxx_src/all_SignalHandler.cpp								\
	./cxx_src/all_utils.cpp										\
	./cxx_src/arg_ProgNameGetter.cpp							\
	./cxx_src/arg_SwitchConfigFile.cpp							\
	./cxx_src/conf_AppConfig_0_Ctor_Dtor_Etc.cpp				\
	./cxx_src/conf_AppConfig_1.cpp								\
	./cxx_src/conf_AppConfig_2_file_string_tokens_lexems.cpp	\
	./cxx_src/conf_AppConfig_3_lexems_to_config.cpp				\
	./cxx_src/conf_AppConfig_4_lexems_to_location.cpp			\
	./cxx_src/conf_AppConfig_5_throw_exception.cpp				\
	./cxx_src/conf_AppConfig_6_parser_autoindex.cpp				\
	./cxx_src/conf_AppConfig_6_parser_braces.cpp				\
	./cxx_src/conf_AppConfig_6_parser_cgi_.cpp					\
	./cxx_src/conf_AppConfig_6_parser_cgi_rewrite.cpp			\
	./cxx_src/conf_AppConfig_6_parser_client_max_body.cpp		\
	./cxx_src/conf_AppConfig_6_parser_error_pages.cpp			\
	./cxx_src/conf_AppConfig_6_parser_index_page.cpp			\
	./cxx_src/conf_AppConfig_6_parser_listen.cpp				\
	./cxx_src/conf_AppConfig_6_parser_method.cpp				\
	./cxx_src/conf_AppConfig_6_parser_redirections.cpp			\
	./cxx_src/conf_AppConfig_6_parser_root.cpp					\
	./cxx_src/conf_AppConfig_6_parser_serv_name.cpp				\
	./cxx_src/conf_AppConfig_6_parser_upload.cpp				\
	./cxx_src/conf_AppConfig_utils.cpp							\
	./cxx_src/conf_Config.cpp									\
	./cxx_src/conf_Lexem.cpp									\
	./cxx_src/conf_Location.cpp									\
	./cxx_src/conf_ServerConfig.cpp								\
	./cxx_src/exec_AppLaunch.cpp								\
	./cxx_src/exec_AppRun_01_main_loop.cpp						\
	./cxx_src/exec_AppRun_02_add_delete_from_epoll.cpp			\
	./cxx_src/exec_AppRun_03_start_epoll.cpp					\
	./cxx_src/exec_AppRun_04_update_epoll_buff.cpp				\
	./cxx_src/exec_AppRun_05_handle_event.cpp					\
	./cxx_src/exec_AppRun_06_accept.cpp							\
	./cxx_src/exec_AppRun_07_read.cpp							\
	./cxx_src/exec_AppRun_08_parse_request_line.cpp				\
	./cxx_src/exec_AppRun_0_Ctor_Dtor_Etc.cpp					\
	./cxx_src/exec_AppRun_10_read_header_trailer_delim.cpp		\
	./cxx_src/exec_AppRun_12_parse_headers.cpp					\
	./cxx_src/exec_AppRun_13_parse_uri.cpp						\
	./cxx_src/exec_AppRun_14_parse_header_host.cpp				\
	./cxx_src/exec_AppRun_16_assign_location_to_request.cpp		\
	./cxx_src/exec_AppRun_18_read_body.cpp						\
	./cxx_src/exec_AppRun_19_read_trailers.cpp					\
	./cxx_src/exec_AppRun_20_response.cpp						\
	./cxx_src/exec_AppRun_30_close.cpp							\
	./cxx_src/exec_AppRun_40_kill_timeout.cpp					\
	./cxx_src/exec_AppRun_50_prepare_response.cpp				\
	./cxx_src/exec_AppRun_51_assemlbe_chunk_or_unchunk.cpp		\
	./cxx_src/exec_AppRun_52_prepare_error.cpp					\
	./cxx_src/exec_AppRun_53_prepare_chunk.cpp					\
	./cxx_src/exec_AppRun_54_prepare_redirect.cpp				\
	./cxx_src/exec_AppRun_56_prepare_cgi.cpp					\
	./cxx_src/exec_AppRun_57_cgi_launch.cpp						\
	./cxx_src/exec_AppRun_58_cgi_catch.cpp						\
	./cxx_src/exec_AppRun_59_process_cgi_response.cpp			\
	./cxx_src/exec_AppRun_60_prepare_get.cpp					\
	./cxx_src/exec_AppRun_65_prepare_post.cpp					\
	./cxx_src/exec_AppRun_70_prepare_delete.cpp					\
	./cxx_src/exec_AppRun_utils_content_type.cpp				\
	./cxx_src/exec_AppRun_utils_delete_handler_from_map.cpp		\
	./cxx_src/exec_AppRun_utils_error_response.cpp				\
	./cxx_src/exec_Epoll_Interface.cpp							\
	./cxx_src/exec_HTTP_1_Listener.cpp							\
	./cxx_src/exec_HTTP_2_Handler.cpp							\
	./cxx_src/exec_HTTP_3_Request.cpp							\
	./cxx_src/exec_HTTP_4_Response.cpp							\
	./cxx_src/exec_HTTP_5_CGI_Request.cpp						\
	./cxx_src/exec_HTTP_6_CGI_Response.cpp

CXXSRC_DIR = ./cxx_src/


HDRS := \
	./incls/wbs_defaults.hpp				\
	./incls/wbs_err_messages.hpp			\
	./incls/wbs_shell_colors.hpp			\
	./incls/wbs_typedef_enum.hpp			\
	./cxx_src/all_AppException.hpp			\
	./cxx_src/all_SignalHandler.hpp			\
	./cxx_src/all_utils.hpp					\
	./cxx_src/arg_ProgNameGetter.hpp		\
	./cxx_src/arg_SwitchConfigFile.hpp		\
	./cxx_src/conf_AppConfig.hpp			\
	./cxx_src/conf_Config.hpp				\
	./cxx_src/conf_Lexem.hpp				\
	./cxx_src/conf_Location.hpp				\
	./cxx_src/conf_ServerConfig.hpp			\
	./cxx_src/exec_AppLaunch.hpp			\
	./cxx_src/exec_AppRun.hpp				\
	./cxx_src/exec_Epoll_Interface.hpp		\
	./cxx_src/exec_HTTP_1_Listener.hpp		\
	./cxx_src/exec_HTTP_2_Handler.hpp		\
	./cxx_src/exec_HTTP_3_Request.hpp		\
	./cxx_src/exec_HTTP_4_Response.hpp		\
	./cxx_src/exec_HTTP_5_CGI_Request.hpp	\
	./cxx_src/exec_HTTP_6_CGI_Response.hpp

CONF := ./confs/test.conf

OBJS := $(CXXSRC:.cpp=.o)
	
CXXFLAGS ?= -O2
override CXXFLAGS += -std=c++98 -Wall -Wextra -Werror -g

override CPPFLAGS += -I./incls -I./cxx_src

.PHONY: all
all: $(NAME)

$(OBJS):	%.o:	%.cpp  $(HDRS) $(CONF)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $<  -o $@


$(NAME): $(OBJS) $(HDRS) Makefile $(CONF)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@echo "$(NAME): compiled"

.PHONY: clean
clean:
	@$(RM) $(OBJS)

.PHONY: fclean
fclean: clean
	@$(RM) $(NAME)

.PHONY: re
re: fclean all