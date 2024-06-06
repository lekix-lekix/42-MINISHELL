/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:26:11 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/06 18:00:14 by kipouliq         ###   ########.fr       */
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
	PAR_RIGHT,
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_OUTPUT_APPEND,
	REDIR_HEREDOC,
    OUTFILE
}					t_token_type;

// typedef enum e_ast_node_type
// {
// 	COMMAND,
// 	OPERATOR
// }					t_ast_node_type;

typedef enum e_redir_type
{
	INPUT,
	OUTPUT,
	OUTPUT_APPEND,
	HEREDOC
}					t_redir_type;

typedef struct s_redir
{
    t_redir_type    redir_type;
    char            *filename;
    struct s_redir  *next;
}                   t_redir;

typedef struct s_token
{
	t_token_type	type;
	t_redir     	*redirections;
	char			*filename;
	char			*content;
	struct s_token	*next;
}					t_token;

typedef struct s_ast
{
	t_token			*token_node;
	t_token_type	node_type;
	int				is_in_par;
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
int					insert_cmd_node(t_ast **tree, t_ast *node);
int					ft_strcpy_sep(char *dest, char *input, char *sep);
void				clear_tree(t_token **tree);
t_ast				*build_ast(t_token **lst, int *insert_node);
void				insert_node_lst(t_token **lst, t_token *node);
void				print_lst(t_token **lst);
t_token				*find_closing_par(t_token **lst);
void				insert_operator_token_node(t_ast **tree, t_ast *node);
// int					parse_insert_cmd_node(t_ast *root, t_ast *cmd_node,
// int level);
t_ast	*handle_par(t_token **lst,
					t_ast **tree,
					/*  t_ast **root,
					*/
					int *insert_node);
t_token				*lst_dup(t_token **lst, t_token *node);
t_ast				*create_ast_node(t_token *node);
t_token				*find_operator_token(t_token **lst);
void				syntax_error(t_token *node);
t_ast				*get_first_node_tree(t_ast *root);
int					check_tree_syntax(t_ast **tree);
int					ft_is_space(char c);
char				*skip_spaces(char *str);
int					is_an_operator(char c);
char				*msh_strtrim(char const *s1, char const *set);
char				**msh_split(char const *s, char c, int mlc_list);
t_token				*create_cmd_node(char *input, char *sep);

#endif
