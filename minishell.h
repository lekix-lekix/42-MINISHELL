/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:26:11 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/01 17:54:59 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./42-MEGALIBFT/megalibft.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
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
	ARGS_FLAGS,
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
	t_token_type	redir_type;
	char			*filename;
	struct s_redir	*next;
}					t_redir;

typedef struct s_token
{
	t_token_type	type;
	t_redir			*redirections;
	char			*filename;
	char			*content;
	char			**contents;
	struct s_token	*next;
}					t_token;

typedef struct s_ast
{
	t_token			*token_node;
	t_token_type	node_type;
	t_redir			*redirections;
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
	t_token			*les_token;
	int				exit_status;
	char			*prompt;
	char			*path;
	int				*pids;
	int				pids_num;
	int				stdin;
	int				stdout;
	int				heredoc;
	t_ast			*node;
	bool			signint_child;
	bool			heredoc_sigint;
	struct termios	original_term;
}					t_minishell;

t_minishell			*ft_shell(void);

int					gbg_coll(void *mem_addr, int which_list, int rule);
t_env				*get_env_lst(char **envp);
int					ft_strlen_sep(char *str, char *sep);

// tokenization
int					trim_token_fields(t_token **lst);
t_token				*tokenize_input(char *input);

// Merge combined functions
void				print_tree(t_ast **tree);
char				*find_operator(char *str);
int					insert_cmd_node(t_ast **tree, t_ast *node);
int					ft_strcpy_sep(char *dest, char *input, char *sep);
void				create_consume_insert_node(t_token **lst, t_token **node,
						t_ast **tree, t_ast **tree_right);
t_ast				*build_ast(t_token **lst, int *insert_node);
void				insert_node_lst(t_token **lst, t_token *node);
void				print_lst(t_token **lst);
void				insert_operator_token_node(t_ast **tree, t_ast *node);
t_ast				*handle_par(t_token **lst, t_ast **tree, int *insert_node);
t_token				*lst_dup(t_token **lst, t_token *node);
t_ast				*create_ast_node(t_token *node);
t_token				*find_operator_token(t_token **lst);
t_ast				*get_first_node_tree(t_ast *root);
int					ft_is_space(char c);
char				*skip_spaces(char *str);
int					is_an_operator(char c);
char				*msh_strtrim(char const *s1, char const *set);
char				**msh_split(char const *s, char c, int mlc_list);
t_token				*create_cmd_node(char *input, char *sep);
int					check_redirections(t_token **lst);
char				*get_filename(t_token *node);
t_token				*find_redir_node(t_token **lst, t_token *redir_node);
void				remove_token_node(t_token **lst, t_token *node);
int					only_spaces(char *str);
void				gbg_delete_node(t_token *node, int mlc_lst);
int					clean_token_lst(t_token **lst);
int					is_a_redir_operator(t_token *node);
void				split_lst_contents(t_token **lst);
int					is_a_token_operator(t_token *node);
char				*msh_strdup(const char *s, int mlc_lst);
void				print_redir_lst(t_redir **lst);

// The builtins
int					ft_exec_echo(char **args);
int					ft_exec_pwd(void);
int					ft_do_cd(char **path, t_minishell *data);
void				ft_update_envlst(char *field, char *content, bool create,
						t_minishell *data);
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

// The non-builtins
int					ft_exec_non_builtins(char **args, t_minishell *data,
						t_redir *redirections);

// utils
void				ft_print_err(char *str);
int					ft_start_exec(t_ast **tree, t_minishell *data);
void				ft_free(char **arr);
void				ft_print_err(char *str);
char				**env_lst_to_arr(t_env **lst);
char				*ft_join(char *s1, char *s2);
int					check_operator_len(char *str);
char				*skip_spaces(char *str);
int					print_env(t_env **lst);
char				*msh_strdup(const char *s, int mlc_lst);

// paths utils
char				*ft_check_path(char *cmd, char **env);
void				lst_env_add_back(t_env **lst, t_env *new);
void				consume_node(t_token **lst, t_token *node);
int					parse_insert_cmd_node(t_ast *root, t_ast *cmd_node,
						int level);
// print syntax errors
int					print_newline_syntax_error(void);
int					print_char_syntax_error(char *str);
int					print_ast_syntax_error(t_ast *node);
int					print_token_syntax_error(t_token *node);

// handle args
void				join_cmd_args(t_token **lst);

// Check syntax
int					check_tree_syntax(t_ast **tree);
int					print_ast_syntax_error(t_ast *node);
int					check_par_syntax(t_token **lst);
int					check_quotes(char *str);
int					check_redir_syntax(t_token **input);

// ast parenthesis
t_token				*find_closing_par(t_token **lst);
t_token				*find_right_par(t_token **lst);

// garbage collector
void				remove_mem_node(t_lst **lst, void *mem_addr);
void				print_tree(t_ast **tree); // to remove

// parsing redirections

t_redir				*create_redir_node(t_token_type redir_type, char *filename);
t_redir				*redir_lst_dup(t_redir **lst);
t_redir				*find_last_redir_node(t_redir **lst);
t_token				*get_outfile_next_node(t_token **lst);
void				add_redirection_node(t_redir **lst, t_redir *node);
int					add_redirection(t_token *cmd_node, t_token *redir_node,
						char *filename);
void				set_redir_lst(t_token **lst);
void				set_args_lst(t_token **lst);

// print functions
void				print_redir_lst(t_redir **lst);

void	find_operator_type(char *input, t_token *node);

void				lst_env_add_back(t_env **lst, t_env *new);
void				consume_node(t_token **lst, t_token *node);
int					parse_insert_cmd_node(t_ast *root, t_ast *cmd_node,
						int level);
int					ft_check_redirections(t_redir *redirections,
						t_minishell *data);
bool				ft_is_delimiter(char *delimiter, char *str);
void				ft_init_tree(t_ast *data);
void	ft_reset_ports(bool piped);

// Expanders
void				ft_heredoc_expander(char *str, int fd);

#endif
