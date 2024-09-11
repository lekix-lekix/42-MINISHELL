# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/18 14:55:41 by sabakar-          #+#    #+#              #
#    Updated: 2024/09/11 20:10:14 by sabakar-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = ./srcs/minishell.c \
	./srcs/garbage_collector.c \
	./srcs/env.c \
	./srcs/execs/exec_builtins.c \
	./srcs/Builtins/do_cd.c \
	./srcs/Builtins/do_echo.c \
	./srcs/Builtins/do_pwd.c \
	./srcs/Builtins/env_utils.c \
	./srcs/Builtins/ft_export.c \
	./srcs/Builtins/ft_unset.c \
	./srcs/execs/exec_non_builtins.c \
	./srcs/execs/start_exec.c \
	./srcs/execs/exec_ast_tools.c \
	./srcs/execs/exec_ast_tools_bis.c \
	./srcs/execs/exec_handle_par.c \
	./srcs/execs/exec_init_child.c \
	./srcs/execs/init_tree.c \
	./srcs/execs/exec_redirections.c \
	./srcs/execs/exec_utils.c \
	./srcs/execs/exec_pipe_redirections.c \
	./srcs/execs/exec_traverse_tree.c \
	./srcs/execs/prep_exec_child.c \
	./srcs/execs/set_close_pipes.c \
	./srcs/execs/set_close_pipes_bis.c \
	./srcs/execs/exec_handle_par_utils.c \
	./srcs/execs/exec_handle_par_utils_bis.c \
	./srcs/general_utils.c \
	./srcs/non_builtins/paths_utils.c \
	./srcs/tokenizer.c \
	./srcs/token_lst_tools.c \
	./srcs/parsing_tools.c \
	./srcs/parsing_tools_bis.c \
	./srcs/ast_construction.c \
	./srcs/ast_parenthesis.c \
	./srcs/ast_insert_node.c \
	./srcs/ast_check_syntax.c\
	./srcs/ast_tools.c \
	./srcs/ast_tools_bis.c \
	./srcs/msh_strtrim.c \
	./srcs/msh_split.c \
	./srcs/msh_strtrim_spaces.c \
	./srcs/expanders/heredoc_expander.c \
	./srcs/parsing_redir.c \
	./srcs/parsing_redir_bis.c \
	./srcs/parsing_redir_utils.c \
	./srcs/parsing_redir_utils_bis.c \
	./srcs/parsing_split_token_content.c \
	./srcs/print_syntax_errors.c \
	./srcs/ast_print.c \
	./srcs/ast_par_utils.c \
	./srcs/garbage_collector_bis.c \
	./srcs/str_manipulation.c \
	./srcs/parsing_clean_tokens.c \
	./srcs/parsing_check_syntax.c \
	./srcs/print_funcs.c \
	./srcs/parsing_handle_args.c \
	./srcs/expanders/ft_expands.c \
	./srcs/expanders/expands_utils.c \
	./srcs/expanders/ft_remove_quotes.c \
	./srcs/expanders/ft_asterisker.c \
	./srcs/expanders/ft_globaler.c \
	./srcs/signals.c \
	./srcs/Builtins/ft_exit.c \
	./srcs/expanders/ft_handle_money.c

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
	rm -f ./srcs/Builtins/*o
	rm -f ./srcs/execs/*o
	rm -f ./srcs/non_builtins/*o
	rm -f ./srcs/expanders/*o
	
fclean : clean
	make -sC $(PATH_LIBFT) fclean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re