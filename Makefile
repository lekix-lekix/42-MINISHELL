# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lekix <lekix@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/18 14:55:41 by sabakar-          #+#    #+#              #
#    Updated: 2024/09/22 17:26:21 by lekix            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = ./srcs/minishell.c \
	./srcs/garbage_collector/garbage_collector.c \
	./srcs/garbage_collector/garbage_collector_bis.c \
	./srcs/Builtins/env.c \
	./srcs/Builtins/do_cd.c \
	./srcs/Builtins/do_echo.c \
	./srcs/Builtins/do_pwd.c \
	./srcs/Builtins/env_utils.c \
	./srcs/Builtins/env_utils_bis.c \
	./srcs/Builtins/ft_export.c \
	./srcs/Builtins/ft_export_bis.c \
	./srcs/Builtins/ft_export_concat.c \
	./srcs/Builtins/ft_unset.c \
	./srcs/Builtins/ft_exit.c \
	./srcs/execs/exec_builtins.c \
	./srcs/execs/exec_non_builtins.c \
	./srcs/execs/start_exec.c \
	./srcs/execs/exec_ast_tools.c \
	./srcs/execs/exec_ast_tools_bis.c \
	./srcs/execs/exec_handle_par.c \
	./srcs/execs/exec_init_child.c \
	./srcs/heredoc/init_tree_heredoc.c \
	./srcs/heredoc/heredoc_filename.c \
	./srcs/execs/exec_redirections.c \
	./srcs/execs/exec_utils.c \
	./srcs/execs/exec_utils_bis.c \
	./srcs/execs/exec_pipe_redirections.c \
	./srcs/execs/exec_traverse_tree.c \
	./srcs/execs/prep_exec_child.c \
	./srcs/execs/set_close_pipes.c \
	./srcs/execs/set_close_pipes_bis.c \
	./srcs/execs/exec_handle_par_utils.c \
	./srcs/execs/exec_handle_par_utils_bis.c \
	./srcs/path/paths_checking.c \
	./srcs/path/path_errors.c \
	./srcs/tokenizer/tokenizer.c \
	./srcs/tokenizer/token_create_nodes.c \
	./srcs/tokenizer/token_lst_tools.c \
	./srcs/abstract_syntax_tree/ast_construction.c \
	./srcs/abstract_syntax_tree/ast_parenthesis.c \
	./srcs/abstract_syntax_tree/ast_insert_node.c \
	./srcs/abstract_syntax_tree/ast_check_syntax.c\
	./srcs/abstract_syntax_tree/ast_check_syntax_bis.c \
	./srcs/abstract_syntax_tree/ast_tools.c \
	./srcs/abstract_syntax_tree/ast_tools_bis.c \
	./srcs/abstract_syntax_tree/ast_par_utils.c \
	./srcs/utils/general_utils.c \
	./srcs/utils/general_utils_bis.c \
	./srcs/utils/msh_strtrim.c \
	./srcs/utils/msh_split.c \
	./srcs/utils/msh_strtrim_spaces.c \
	./srcs/utils/msh_split_spaces.c \
	./srcs/utils/print_syntax_errors.c \
	./srcs/utils/print_funcs.c \
	./srcs/utils/str_manipulation.c \
	./srcs/utils/signals.c \
	./srcs/parsing/parsing_redir_bis.c \
	./srcs/parsing/parsing_redir_utils.c \
	./srcs/parsing/parsing_redir_utils_bis.c \
	./srcs/parsing/parsing_split_token_content.c \
	./srcs/parsing/parsing_word_utils.c \
	./srcs/parsing/parsing_find_type.c \
	./srcs/parsing/parsing_clean_tokens.c \
	./srcs/parsing/parsing_check_syntax.c \
	./srcs/parsing/parsing_tools.c \
	./srcs/parsing/parsing_redir.c \
	./srcs/parsing/parsing_tools_bis.c \
	./srcs/parsing/parsing_handle_args.c \
	./srcs/expanders/heredoc_expander.c \
	./srcs/expanders/ft_handle_money.c \
	./srcs/expanders/ft_expands.c \
	./srcs/expanders/expands_utils.c \
	./srcs/expanders/ft_remove_quotes.c \
	./srcs/expanders/ft_asterisker.c \
	./srcs/expanders/ft_globaler.c \
	./srcs/expanders/ft_expand_cmd.c \
	./srcs/init_data.c

OBJ = $(SRCS:.c=.o)

OBJ_BONUS = $(SRCS_BONUS:.c=.o)

CC = cc

FLAGS = -Wall -Wextra -Werror -g3

PATH_LIBFT = ./42-MEGALIBFT

LIBFT = ./42-MEGALIBFT/megalibft.a

LIBS = -lreadline

all : 
	git submodule update --init
	make $(NAME)

$(NAME) : $(OBJ)
	make -C $(PATH_LIBFT)
	$(CC) $(FLAGS) $(OBJ) $(LIBFT) -o $(NAME) -g3 $(LIBS)

%.o:%.c
	$(CC) $(FLAGS) -I $(PATH_LIBFT) -c $< -o $@

clean :
	make -sC $(PATH_LIBFT) clean
	rm -f ./srcs/*o
	rm -f ./srcs/garbage_collector/*o
	rm -f ./srcs/Builtins/*o
	rm -f ./srcs/abstract_syntax_tree/*o
	rm -f ./srcs/heredoc/*o
	rm -f ./srcs/tokenizer/*o
	rm -f ./srcs/utils/*o
	rm -f ./srcs/parsing/*o
	rm -f ./srcs/execs/*o
	rm -f ./srcs/path/*o
	rm -f ./srcs/expanders/*o
	
fclean : clean
	make -sC $(PATH_LIBFT) fclean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re