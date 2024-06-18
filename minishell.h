/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:26:11 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/17 12:02:43 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./42-MEGALIBFT/megalibft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define PER_ERR \
	"Permission denied, you can't \
open this file! Or file doesn't exit, Or it's a directory"
# define CMD_ERR "Command not found!"
# define OPEN_ERR "Error opening file"
# define DUP_ERR "An err has occoured with dup2"
# define FORK_C2 "An Error has occuered with Fork child2"
# define FORK_C1 "An Error has occuered with Fork child1"
# define BASH_ERR "bash: "

typedef struct s_env
{
	char			*field;
	char			*content;
	struct s_env	*next;
}					t_env;

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

typedef enum e_err_no
{
	ENO_SUCCESS,
	ENO_GENERAL,
	ENO_NOT_FOUND,
	ENO_CANNOT_EXEC = 126,
	ENO_EXEC_255 = 255,
}					t_err_no;

typedef enum e_cmd_pipe_directions
{
	P_CMD_LEFT,
	P_CMD_RIGHT
}					t_cmd_pipe_directions;

typedef struct s_minishell
{
	t_env			*env_lst;
	int				exit_status;
	char			*prompt;
	int				*pids;
	int				pids_num;
}					t_minishell;

// extern t_minishell	g_minishell;

int					gbg_coll(void *mem_addr, int which_list, int rule);
t_env				*get_env_lst(char **envp);
int					ft_strlen_sep(char *str, char *sep);
t_token				*tokenize_input(char *input);
// void				perint_tree(t_ast **tree);
int					ft_strlen_sep(char *str, char *sep);
char				*find_operator(char *str);
// void				insert_operator_node(t_token **tree, t_token *node);
// void				insert_cmd_node(t_token **tree, t_token *node);
int					ft_strcpy_sep(char *dest, char *input, char *sep);
// void				clear_tree(t_token **tree);
t_ast				*build_ast(t_token **lst);
t_env				*create_env_node(char *str, char *sep);
void				lst_env_add_back(t_env **lst, t_env *new);

// The builtins
int					ft_exec_echo(char **args);
int					ft_exec_pwd(void);
int					ft_do_cd(char **path, t_minishell *data);
void				ft_update_envlst(char *field, char *content, bool create, t_minishell *data);
int					print_env(t_env **lst);
int					ft_exec_export(char **args, t_minishell *data);
int					ft_exec_builtins(char **args, t_minishell *data);
char				*ft_extract_val(char *str);
char				*ft_extract_key(char *str);
int					ft_strcmp(const char *s1, const char *s2);
bool				ft_is_builtin(char *arg);
bool				ft_env_entry_exists(char *content, t_minishell *data);
int					ft_exec_unset(char **args, t_minishell *data);
int					ft_check_key(char *str);
char				*ft_get_envlst_content(char *content, t_minishell *data);

// The none builtins
int					ft_exec_non_builtins(char **args, t_minishell *data);
// int					ft_exec_node(char **args, char *lat_path, char **env);

// utils
void				ft_print_err(char *str);
int					ft_start_exec(t_ast **tree, t_minishell *data);
void				ft_free(char **arr);
void				ft_print_err(char *str);
char				**env_lst_to_arr(t_env **lst);
// void				print_lst(t_token **lst);

// paths utils
char				*ft_check_path(char *cmd, char **env);

#endif
