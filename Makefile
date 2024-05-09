# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/02/26 17:19:05 by kipouliq          #+#    #+#              #
#    Updated: 2024/05/09 16:08:16 by kipouliq         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = minishell

SRCS = ./srcs/minishell.c \
	./srcs/garbage_collector.c

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

fclean : clean
	make -sC $(PATH_LIBFT) fclean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)

re : fclean
	make all

.PHONY : all bonus clean fclean re