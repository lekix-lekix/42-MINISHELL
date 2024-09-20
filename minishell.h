/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:51:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/20 14:08:59 by kipouliq         ###   ########.fr       */
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
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <termios.h>
# include <unistd.h>

# define PER_ERR "Permission denied"
# define NOT_FOUND_ERR "No such file or directory"
# define CMD_ERR "command not found"
# define OPEN_ERR "Error opening file"
# define IS_DIR_ERR "Is a directory"
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
	int				heredoc;
	struct s_redir	*next;
}					t_redir;

typedef struct s_token
{
	t_token_type	type;
	t_redir			*redirections;
	int				pipe_redir[2];
	char			*filename;
	char			*content;
	char			**contents;
	int				is_in_par;
	struct s_token	*original_token;
	struct s_token	*next;
}					t_token;

typedef struct s_ast
{
	t_token			*token_node;
	t_token_type	node_type;
	t_redir			*redirections;
	char			**expanded_args;
	int				is_in_par;
	struct s_ast	*left;
	struct s_ast	*right;
	struct s_ast	*next;
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
	t_lst			*pids;
	t_lst			*pipes;
	int				end_exec;
	int				exec_in_par;
	int				pids_num;
	int				ft_stdin;
	int				ft_stdout;
	int				msh_stdout;
	t_ast			*full_exec_tree;
	t_ast			*exec_tree;
	t_ast			*node;
	bool			signint_child;
	bool			heredoc_sigint;
	char			**env_args;
	t_env			*expanded_env;
	char			**expanded_args;
	int				expand_chars_trimmed;
	char			*tmp_file_path;
	struct termios	original_term;
}					t_minishell;

t_minishell			*ft_shell(void);
int					init_data(t_minishell *data, char **envp);
int					start_parsing(char *prompt);
int					gbg_coll(void *mem_addr, int which_list, int rule);
t_env				*get_env_lst(char **envp);
int					ft_strlen_sep(char *str, char *sep);
int					ft_strcpy_sep_ptr(char *dest, char *input, char *sep);

//
void				ft_close_fds(void);

// Init data
int					init_data(t_minishell *data, char **envp);
char				*get_path(char **envp);

// tokenization
int					trim_token_fields(t_token **lst);
t_token				*tokenize_input(char *input);
char				*msh_strtrim_spaces(char const *s1);
void				create_insert_token_nodes(t_token **lst, char **input,
						char **operator);
int					is_a_quote(char c);
void				more_tokenization(t_token **lst);
int					check_delete_global_par(t_token **lst);

// Merge combined functions
char				*find_operator(char *str);
int					insert_cmd_node(t_ast **tree, t_ast *node);
int					ft_strcpy_sep(char *dest, char *input, char *sep);
void				create_consume_insert_node(t_token **lst, t_token **node,
						t_ast **tree, t_ast **tree_right);
t_ast				*build_ast(t_token **lst, int *insert_node);
void				insert_node_lst(t_token **lst, t_token *node);
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

void				remove_token_node(t_token **lst, t_token *node);
int					only_spaces(char *str);
void				gbg_delete_node(t_token *node, int mlc_lst);
int					clean_token_lst(t_token **lst);
int					split_lst_contents(t_token **lst);
int					is_a_token_operator(t_token *node);
char				*msh_strdup(const char *s, int mlc_lst);

// The builtins
int					ft_exec_echo(char **args);
int					ft_exec_pwd(char **args);
int					ft_do_cd(char **path);
void				ft_update_envlst(char *field, char *content, bool create);
int					print_env(t_env **lst);
int					ft_exec_export(char **args);
int					ft_exec_builtins(char **args);
char				*ft_extract_field(char *str);
char				*ft_extract_content(char *str);
int					ft_strcmp(const char *s1, const char *s2);
bool				ft_is_builtin(char *arg);
bool				ft_env_entry_exists(char *content);
int					ft_exec_unset(char **args);
void				*ft_unset_cleaner(void *ptr, bool clean);
int					ft_check_key(char *str);
char				*ft_get_envlst_content(char *content, t_env **env_lst);
char				*get_env_content(char *field);
void				ft_export_list(t_env **envlst);
int					check_and_update_envlst(char **args, int *exit_s, int i);
t_env				*env_cpy_lst(t_env **envlst);

// The non-builtins
int					ft_exec_non_builtins(t_token *node);
int					ft_strncmp_loco(const char *s1, const char *s2, size_t n);

// utils
void				ft_print_err(char *str);
int					ft_start_exec(t_ast **tree);
void				ft_free(char **arr);
void				ft_print_err(char *str);
char				**env_lst_to_arr(t_env **lst);
char				*ft_join(char *s1, char *s2);
int					check_operator_len(char *str);
char				*skip_spaces(char *str);
int					print_env(t_env **lst);
char				*msh_strdup(const char *s, int mlc_lst);
int					content_count_words(char *str);
char				*get_next_word(char **input_str);
char				**msh_split_spaces(char const *s, int mlc_list);
char				**ft_concat_str_arr(char **arr, char **arr2);
char				*empty_str(void);
int					ft_count_words(char const *s, char sep);
char				*create_random_filename(void);
char				**dup_arr_join_empty_str(char **arr);
char				**ft_concat_str_arr_idx(char **arr, char **arr2);
int					str_contains_expand(char *str);
int					str_contains_spaces(char *str);
void				trim_contents(t_token **lst);

// paths utils
char				*ft_check_path(char **contents, char **env,
						int *exit_status, int *i);
void				lst_env_add_back(t_env **lst, t_env *new);
void				consume_node(t_token **lst, t_token *node);
int					parse_insert_cmd_node(t_ast *root, t_ast *cmd_node,
						int level);
int					print_msh_error(char *err, char *cmd);
char				*dot_error(void);

// print syntax errors
int					print_newline_syntax_error(void);
int					print_char_syntax_error(char *str);
int					print_ast_syntax_error(t_ast *node);
int					print_token_syntax_error(t_token *node);

// handle args
void				join_cmd_args(t_token **lst);

// Check syntax
void				get_last_node_tree(t_ast *root, t_ast **node);
int					check_tree_syntax(t_ast **tree);
int					print_ast_syntax_error(t_ast *node);
int					check_par_syntax(t_token **lst);
int					check_quotes(char *str);
int					check_redir_syntax(t_token **input);
int					check_pipes_par_syntax(t_token **lst);

// ast parenthesis
t_token				*find_closing_par(t_token **lst);
t_token				*find_right_par(t_token **lst);

// garbage collector
void				remove_mem_node(t_lst **lst, void *mem_addr);

// parsing redirections

t_redir				*create_redir_node(t_token_type redir_type, char *filename);
t_redir				*redir_lst_dup(t_redir **lst);
t_redir				*find_last_redir_node(t_redir **lst);
t_token				*get_outfile_next_node(t_token **lst);
void				add_redirection_node(t_redir **lst, t_redir *node);
void				set_redir_lst(t_token **lst);
void				set_args_lst(t_token **lst);
char				*get_filename(t_token *node);
int					check_redirections(t_token **lst);
t_token				*find_redir_node(t_token **lst, t_token *redir_node);
int					is_a_redir_operator(t_token *node);
void				add_front_redir_node(t_redir **lst, t_redir *node);
t_redir				*get_redir_lst_par(t_token **redir_node_lst);

// Parsing utils
int					get_end_word_idx(char *str, int i);

// print functions
// void				print_redir_lst(t_redir **lst);

void				find_operator_type(char *input, t_token *node);

void				lst_env_add_back(t_env **lst, t_env *new);
int					parse_insert_cmd_node(t_ast *root, t_ast *cmd_node,
						int level);
int					ft_check_redirections(t_token *node);
bool				ft_is_delimiter(char *delimiter, char *str);
int					ft_init_tree(t_ast *data);
void				ft_reset_ports(bool piped);

// Expanders
void				ft_heredoc_expander(char *str, int fd);
bool				ft_is_valid_var_char(char c);
char				**ft_expand(char *sr);
char				*ft_strip_quotes(char *str);
char				*ft_clean_empty_chars(char *sr);
bool				ft_is_contains_asterisk(char *sr);
size_t				ft_count_match(char *pattern);
bool				ft_match_star(char *pattern, char *str);
char				**ft_globaler(char *str);
size_t				get_arr_len(char **expanded);
void				expand_redirections(t_redir **redirections);
char				*ft_pre_expand(char *sr);
char				*ft_handle_dollar(char *str, size_t *i);
char				*ft_handle_dquote_str(char *str, size_t *i);
char				*ft_check_squotes(char *sr, size_t *x);
int					expand_cmd(t_token *cmd);

// Exec AST tools
void				set_is_in_par(t_ast *root, int flag);
void				add_ast_lst(t_ast **ast_lst, t_ast *node);
t_ast				*find_top_node(t_ast **lst);
void				set_next_null(t_ast *root);
t_ast				*ast_lst_dup(t_ast **lst, t_ast *node);
t_ast				*get_after_par_node(t_ast **lst);
t_ast				*ast_find_one(t_ast **lst, t_ast *node);
int					ast_list_size(t_ast **lst);

// Exec pipe redirections
int					handle_pipe_redirections(t_redir *redirection,
						t_token *node);
int					close_pipes_lst(t_lst **pipe_lst);
void				set_pipe_redir_out(t_ast *node, int pipe_fd);
void				set_pipe_redir_in(t_ast *node, int pipe_fd);
int					close_pipe_redir_in(t_ast *node);
void				close_pipe_redir_out(t_ast *node);
int					*init_pipe(void);
t_lst				*create_lst_node(void *mem_addr);
void				ft_lst_del_pipe(t_lst **lst, int pipe);

// Exec
int					init_only_child_no_fork(t_token *node);
int					ft_check_cmds(t_token *token_node);
int					*prep_exec_child(t_ast *to_exec);
int					close_wait(int par_pid);
void				ft_start_exec_tree(t_ast *root, t_ast **exec_lst,
						t_ast **last_op);
int					iterate_exec_ast_lst(t_ast **lst);
int					ft_exit_close(int exit_status);

// Exec parenthesis
int					prep_exec_par(t_ast *sub_tree, int *after_par_pipe);
int					handle_par_exec(t_ast **current);
void				set_par_lst(t_token **lst);
t_token				*find_original_token_lst(t_token **lst, t_token *to_find);
void				delete_begin_end_par_nodes(t_token **lst);
void				set_par_lst(t_token **lst);
t_token				*find_left_par_original_token(t_token **lst, t_token *node);
t_token				*check_after_par_pipe(t_token **par_lst,
						int **after_par_pipe);
void				set_back_redir(t_ast **lst, t_token **lst_dup);
t_ast				*find_token_node(t_ast **lst, t_token *to_find);

// Signals
void				ft_init_signals(void);
void				ft_sigquit_handler(int num);
void				ft_exit(char **args);
void				ft_sigint_handler_heredoc(int num);

#endif