/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:26:11 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/09 16:07:46 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./42-MEGALIBFT/megalibft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>

int	gbg_coll(void *mem_addr, int which_list, int rule);

typedef struct s_env
{
	char			*field;
	char			*content;
	struct s_env	*next;
}					t_env;

typedef struct s_minishell
{
	t_env			*env_lst;
	char			*path;
}					t_minishell;

typedef struct s_gbg
{
	t_lst			*mlc_env;
	t_lst			*mlc_parsing;
}					t_gbg;

typedef enum e_gbg_rules
{
	ADD,
	FREE,
	FLUSH_ONE,
    FLUSH_ALL
}					t_gbg_rules;

typedef enum e_gbg_lst_names
{
    ENV,
    PARSING,
    ALL
}                   t_gbg_lst_names;

#endif