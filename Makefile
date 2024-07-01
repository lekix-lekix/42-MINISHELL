# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/18 14:55:41 by sabakar-          #+#    #+#              #
#    Updated: 2024/07/01 05:49:39 by sabakar-         ###   ########.fr        #
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
	./srcs/execs/exec_non_builtins.c \
	./srcs/Builtins/ft_unset.c \
	./srcs/execs/start_exec.c \
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
	./srcs/parsing_redirections.c \
	./srcs/parsing_redirections_bis.c \
	./srcs/execs/exec_redirections.c \
	./srcs/execs/init_tree.c \
	./srcs/execs/exec_utils.c \
	./srcs/expanders/heredoc_utils.c 

	
	# .srcs/execs/heredoc_expander.c \
	# ./srcs/ft_tree_helpers.c \
	#	./srcs/ft_extra_fts.c \

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
	# make clean	

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
	
fclean : clean
	make -sC $(PATH_LIBFT) fclean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re