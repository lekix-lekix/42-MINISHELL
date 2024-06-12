# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/06/11 14:27:14 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = ./srcs/minishell.c \
	./srcs/garbage_collector.c \
	./srcs/env.c \
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
	./srcs/parsing_redirections_bis.c

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

fclean : clean
	make -sC $(PATH_LIBFT) fclean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re