/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/06 16:13:38 by lekix            ###   ########.fr       */
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
	// printf("%s\n", data->path);[]
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
	// print_tree(&nodes);
	ft_init_tree(nodes);
	ft_shell()->pids = NULL;
	ft_shell()->pipes = NULL;
	ft_shell()->end_exec = 0;
	ft_shell()->exec_in_par = 0;
	ft_shell()->full_exec_tree = *tree;
	ft_shell()->ft_stdin = dup(STDIN_FILENO);
	ft_shell()->ft_stdout = dup(STDOUT_FILENO);
	if ((ft_shell())->heredoc_sigint)
	{
		// If the program is quited during heredoc,
		// we have clean the mess afterword I guess
		printf(" HERE 98\n");
		// gbg_coll(NULL, PARSING, FLUSH_ONE);
		ft_close_fds();
		gbg_coll(NULL, ALL, FLUSH_ALL);
		// gbg_coll(NULL, ENV, FLUSH_ALL);
		// clean_token_lst(&nodes->token_node);
		(ft_shell())->heredoc_sigint = false;
		return ;
	}
	tcsetattr(STDIN_FILENO, TCSANOW, &(ft_shell())->original_term);
	ft_start_exec(&nodes);
}

void	gbg_delete_node(t_token *node, int mlc_lst)
{
	gbg_coll(node->content, mlc_lst, FREE);
	gbg_coll(node, mlc_lst, FREE);
}

void	check_delete_global_par(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	if (*lst && (*lst)->type == PAR_LEFT && find_closing_par(lst)->next == NULL)
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
}

t_token	*create_cmd_node_no_sep(char *input)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node || gbg_coll(node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	node->content = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!node->content || gbg_coll(node->content, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	ft_strlcpy(node->content, input, ft_strlen(input) + 1);
	node->redirections = NULL;
	node->pipe_redir[0] = -1;
	node->pipe_redir[1] = -1;
	node->contents = NULL;
	node->original_token = NULL;
	node->type = CMD;
	node->filename = NULL;
	node->next = NULL;
	return (node);
}

void	more_tokenization(t_token **lst)
{
	t_token	*current;
	t_token	*new_cmd;
	t_token	*prev;
	char	*new_content;

	current = *lst;
	while (current)
	{
		// printf("words = %d\n", content_count_words(current->content));
		if (content_count_words(current->content) > 1)
		{
			// printf("yo\n");
			new_content = get_next_word(&current->content);
			new_cmd = create_cmd_node_no_sep(new_content);
			new_cmd->next = current;
			if (current == *lst)
				*lst = new_cmd;
			else
				prev->next = new_cmd;
			current = *lst;
			continue ;
		}
		prev = current;
		current = current->next;
	}
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
	// printf("after more tokenization =======\n");
	// print_lst(&input);
	// printf("=======\n");
	trim_contents(&input);
	set_redir_lst(&input);
	// printf("after set redir lst  =======\n");
	// print_lst(&input);
	// printf("=======\n");
	if (check_redir_syntax(&input) == -1 || check_par_syntax(&input) == -1)
		return (ft_shell()->exit_status = 2, -1);
	if (check_redirections(&input) == -1)
		return (ft_shell()->exit_status = 2, -1);
	// dprintf(2, "ddfsdf\n");
	// printf("after check redir =======\n");
	// print_lst(&input);
	// printf("=======\n");
	// printf("==============\n");
	join_cmd_args(&input);
	clean_token_lst(&input);
	check_delete_global_par(&input);
	set_par_lst(&input);
	ft_shell()->les_token = lst_dup(&input, NULL);
	tree = build_ast(&input, NULL);
	if (tree && check_tree_syntax(&tree) == -1)
		return (-1);
	ft_shell()->exec_tree = tree;
	ft_start_execution(&tree);
	ft_close_fds();
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*data;
	char		*line;

	// int			fd;
	// fd = open("./stderr_tmp", O_RDWR);
	// if (dup2(fd, STDERR_FILENO) == -1)
	// 	perror("duppp");
	data = ft_shell();
	((void)argc, (void)argv);
	init_data(data, env);
	while (1)
	{
		ft_init_signals();
		if (isatty(fileno(stdin)))
			data->prompt = readline("minishell$ ");
		else
		{
			line = get_next_line(fileno(stdin), 0);
			data->prompt = ft_strtrim(line, "\n");
			free(line);
		}
		if (!data->prompt)
		{
			// Also we need to clean here if I'm not mistaking!
			gbg_coll(NULL, ALL, FLUSH_ALL);
			// (ft_putstr_fd("exit\n", 1));
			exit(ft_shell()->exit_status);
		}
		if (data->prompt || *data->prompt)
		{
			if (data->prompt[0])
				add_history(data->prompt);
			start_parsing(data->prompt);
			gbg_coll(NULL, PARSING, FLUSH_ONE);
			close(ft_shell()->ft_stdin);
			close(ft_shell()->ft_stdout);
			// gbg_coll(NULL, PARSING, FLUSH_ALL);
			free(data->prompt);
		}
	}
	free(data->prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
