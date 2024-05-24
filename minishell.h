/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:26:11 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/24 17:43:52 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./42-MEGALIBFT/megalibft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>

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

typedef enum e_token_type
{
	CMD,
	PIPE,
	AND,
	OR,
	PAR_LEFT,
	PAR_RIGHT
}					t_token_type;

typedef enum e_ast_node_type
{
	COMMAND,
	OPERATOR
}					t_ast_node_type;

typedef struct s_token
{
	t_token_type	type;
	char			*content;
	struct s_token	*next;
}					t_token;

typedef struct s_ast
{
	t_token			*token_node;
	t_token_type	node_type;
	struct s_ast	*left;
	struct s_ast	*right;
}					t_ast;

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
}					t_gbg_lst_names;

int					gbg_coll(void *mem_addr, int which_list, int rule);
t_env				*get_env_lst(char **envp);
int					ft_strlen_sep(char *str, char *sep);
t_token				*tokenize_input(char *input);
void				print_tree(t_ast **tree);
int					ft_strlen_sep(char *str, char *sep);
char				*find_operator(char *str);
void				insert_operator_node(t_token **tree, t_token *node);
void				insert_cmd_node(t_token **tree, t_token *node);
int					ft_strcpy_sep(char *dest, char *input, char *sep);
void				clear_tree(t_token **tree);
t_ast				*build_ast(t_token **lst);
void				insert_node_lst(t_token **lst, t_token *node);
void				print_lst(t_token **lst);

#endif
