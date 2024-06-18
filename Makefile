# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/06/13 08:08:43 by sabakar-         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = ./srcs/minishell.c \
	./srcs/garbage_collector.c \
	./srcs/env.c \
	./srcs/ast_tools.c \
	./srcs/ast_construction.c \
	./srcs/tokenizer.c \
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
	
fclean : clean
	make -sC $(PATH_LIBFT) fclean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re