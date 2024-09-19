/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/19 11:53:53 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_minishell	*ft_shell(void)
{
	static t_minishell	data;

	return (&data);
}

char	*get_path(char **envp)
{
	char	*str;
	int		i;

	i = -1;
	while (envp && envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			str = ft_strtrim(envp[i], "PATH=");
			if (!str || gbg_coll(str, ENV, ADD) == -1)
				return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(),
					exit(255), NULL);
		}
	}
	return (str);
}

int	init_data(t_minishell *data, char **envp)
{
	data->path = get_path(envp);
	if (!data->path)
		return (-1);
	data->env_lst = get_env_lst(envp);
	data->expanded_env = get_env_lst(envp);
	data->env_args = envp;
	if (!data->env_lst)
		return (-1);
	tcgetattr(STDIN_FILENO, &ft_shell()->original_term);
	return (0);
}

void	traverse_count_pipes(t_ast *root, int *pipes)
{
	if (root->left)
		traverse_count_pipes(root->left, pipes);
	if (root->node_type == PIPE)
		*pipes += 1;
	if (root->right)
		traverse_count_pipes(root->right, pipes);
}

int	ft_count_pipes(t_ast **tree)
{
	int	pipes;

	pipes = 0;
	if (!*tree)
		return (0);
	traverse_count_pipes(*tree, &pipes);
	return (pipes);
}

static void	ft_start_execution(t_ast **tree)
{
	t_ast	*nodes;

	signal(SIGQUIT, ft_sigquit_handler);
	nodes = *tree;
	ft_init_tree(nodes);
	ft_shell()->pids = NULL;
	ft_shell()->pipes = NULL;
	ft_shell()->end_exec = 0;
	ft_shell()->exec_in_par = 0;
	ft_shell()->expand_chars_trimmed = 0;
	ft_shell()->full_exec_tree = *tree;
	ft_shell()->ft_stdin = dup(STDIN_FILENO);
	ft_shell()->ft_stdout = dup(STDOUT_FILENO);
	if ((ft_shell())->heredoc_sigint)
	{
		ft_close_fds();
		printf("It's here 12\n");
		gbg_coll(NULL, ALL, FLUSH_ALL);
		(ft_shell())->heredoc_sigint = false;
		return ;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &(ft_shell())->original_term);
	ft_start_exec(&nodes);
}

int	check_delete_global_par(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	if ((*lst) && (*lst)->type == PAR_LEFT && (*lst)->next->type == PAR_RIGHT)
	{
		printf("bash: syntax error near unexpected token `)'\n");
		return (-1);
	}
	if ((*lst) && (*lst)->type == PAR_LEFT
		&& find_closing_par(lst)->next == NULL)
	{
		*lst = (*lst)->next;
		current = *lst;
		while (current->next)
		{
			prev = current;
			current = current->next;
		}
		prev->next = current;
	}
	return (0);
}

void	trim_contents(t_token **lst)
{
	t_token	*current;

	current = *lst;
	while (current)
	{
		if (current->content)
			current->content = msh_strtrim_spaces(current->content);
		current = current->next;
	}
}

int	expand_token_lst(t_token **lst)
{
	t_token	*io;
	char	**la_args;
	char	**temp_contents;
	int		idx;

	idx = -1;
	temp_contents = NULL;
	io = *lst;
	while (io)
	{
		while (io->type == CMD && io->contents[++idx])
		{
			la_args = ft_expand(io->contents[idx]);
			if (!la_args)
			{
				io->contents[idx] = empty_str();
				break ;
			}
			if (get_arr_len(la_args) == 1)
				io->contents[idx] = la_args[0];
			else
			{
				io->contents[idx] = NULL;
				io->contents = ft_concat_str_arr(io->contents, la_args);
			}
		}
		idx = 0;
		io = io->next;
	}
	return (0);
}

void	print_token_lst(t_token **lst)
{
	int		i;
	t_token	*current;

	current = *lst;
	while (current)
	{
		i = 0;
		printf("=======\n");
		while (current->contents[i])
		{
			printf("contents[%d] = %s\n", i, current->contents[i]);
			i++;
		}
		current = current->next;
	}
}

int	start_parsing(char *prompt)
{
	t_token	*input;
	t_ast	*tree;

	if (!prompt[0])
		return (-1);
	if (check_quotes(prompt))
		return (-1);
	input = tokenize_input(prompt);
	more_tokenization(&input);
	clean_token_lst(&input);
	trim_contents(&input);
	set_redir_lst(&input);
	// print_lst(&input);
	if (check_redir_syntax(&input) == -1 || check_par_syntax(&input) == -1)
		return (ft_shell()->exit_status = 2, -1);
	if (check_redirections(&input) == -1)
		return (ft_shell()->exit_status = 2, -1);
	join_cmd_args(&input);
	clean_token_lst(&input);
	if (check_delete_global_par(&input) == -1)
		return (-1);
	set_par_lst(&input);
	ft_shell()->les_token = lst_dup(&input, NULL);
	// printf("BEFORE EXPAND====\n");
	// print_token_lst(&input);
	expand_token_lst(&input);
	// print_token_lst(&input);
	// printf("AFTER EXPAND====\n");
	tree = build_ast(&input, NULL);
	// print_tree(&tree);
	if (tree && check_tree_syntax(&tree) == -1)
		return (-1);
	ft_shell()->exec_tree = tree;
	(ft_start_execution(&tree), ft_close_fds());
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*data;

	data = ft_shell();
	((void)argc, (void)argv);
	init_data(data, env);
	while (1)
	{
		ft_init_signals();
		data->prompt = readline("minishell$ ");
		if (data->prompt == NULL)
		{
			printf("27\n");
			// Also we need to clean here if I'm not mistaking!
			// printf("WE ARE HERE IN MAIN\n");
			gbg_coll(NULL, ALL, FLUSH_ALL);
			close(ft_shell()->ft_stdin);
			close(ft_shell()->ft_stdout);
			free(data->prompt);
			gbg_coll(NULL, ENV, FLUSH_ALL);
			gbg_coll(NULL, PARSING, FLUSH_ALL);
			gbg_coll(NULL, ENV, FREE);
			rl_clear_history();
			ft_putstr_fd("exit\n", 1);
			exit(ft_shell()->exit_status);
		}
		if (data->prompt && *data->prompt)
		{
			if (data->prompt[0])
				add_history(data->prompt);
			start_parsing(data->prompt);
			gbg_coll(NULL, PARSING, FLUSH_ONE);
			// close(ft_shell()->ft_stdin);
			// close(ft_shell()->ft_stdout);
			// gbg_coll(NULL, PARSING, FLUSH_ALL);
			free(data->prompt);
		}
	}
	free(data->prompt);
	close(ft_shell()->ft_stdin);
	close(ft_shell()->ft_stdout);
	rl_clear_history();
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}

// int	main(int argc, char **argv, char **env)
// {
// 	t_minishell *data;
// 	char *line;

// 	data = ft_shell();
// 	((void)argc, (void)argv);
// 	init_data(data, env);
// 	while (1)
// 	{
// 		ft_init_signals();
// 		if (isatty(fileno(stdin)))
// 			data->prompt = readline("minishell$ ");
// 		else
// 		{
// 			line = get_next_line(fileno(stdin), 0);
// 			data->prompt = ft_strtrim(line, "\n");
// 			free(line);
// 		}
// 		if (!data->prompt)
// 		{
// 			gbg_coll(NULL, ALL, FLUSH_ALL);
// 			exit(ft_shell()->exit_status);
// 		}
// 		if (data->prompt || *data->prompt)
// 		{
// 			if (data->prompt[0])
// 				add_history(data->prompt);
// 			start_parsing(data->prompt);
// 			gbg_coll(NULL, PARSING, FLUSH_ONE);
// 			// close(ft_shell()->ft_stdin);
// 			// close(ft_shell()->ft_stdout);
// 			free(data->prompt);
// 		}
// 	}
// 	free(data->prompt);
// 	gbg_coll(NULL, ENV, FLUSH_ALL);
// 	gbg_coll(NULL, PARSING, FLUSH_ALL);
// 	gbg_coll(NULL, ENV, FREE);
// }