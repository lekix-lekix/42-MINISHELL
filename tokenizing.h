/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizing.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/14 15:17:32 by sabakar-          #+#    #+#             */
/*   Updated: 2024/05/14 15:17:36 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _MINI_SHELL_H
#define _MINI_SHELL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>

typedef struct s_cmds_tree 
{
  int type; // 0 - Command node, 1 - Pipe node, 3 - && operator etc.. but also we can change it to enum vars as you did.
  char *cmd; // Optional value (e.g., command name, redirection filename)
  struct s_cmds_tree *left_kid;
  struct s_cmds_tree *right_kid;
} t_cmds_tree;

typedef struct s_tokens
{
  int type; // 0 - Command, 1 - Operator (not used), 2 - Flag, 3 - Pipe, 4 - Redirection
  char *token; // This is gonna be the token it self
  struct s_tokens *next;
} t_tokens;

void ft_tokenize(char *sr);
t_cmds_tree *ft_parse_tokens(t_tokens *tokens);

#endif