/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/09 17:36:28 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_minishell *ft_shell(void)
{
	static t_minishell data;

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
				return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		}
	}
	return (str);
}

int	init_data(t_minishell *data, char **envp)
{
	data->path = get_path(envp);
	data->stdin = dup(0);
	data->stdout = dup(1);
	if (!data->path)
		return (-1);
	data->env_lst = get_env_lst(envp);
	data->env_args = envp;
	if (!data->env_lst)
		return (-1);
	return (0);
}

static void	ft_start_execution(t_ast **tree)
{
	t_ast	*nodes;
	int		la_status;

	nodes = *tree;
	ft_init_tree(nodes);
	// if (nodes->token_node && nodes->token_node->type == CMD)
	// {
	// 	printf("It was a single cmd\n");
	// 	la_status = ft_exec_non_builtins_single_cmd(nodes->token_node->contents, nodes->redirections);
	// }
	// else
	la_status = ft_start_exec(&nodes);
}

void	gbg_delete_node(t_token *node, int mlc_lst)
{
	gbg_coll(node->content, mlc_lst, FREE);
	gbg_coll(node, mlc_lst, FREE);
}

int	start_parsing(char *prompt)
{
	t_token	*input;
	t_ast	*tree;
	int		insert_node;

	if (!prompt[0])
		return (-1);
	insert_node = 1;
	if (check_quotes(prompt))
		return (-1);
	input = tokenize_input(prompt);
	clean_token_lst(&input);
	if (check_redir_syntax(&input) == -1)
		return (-1);
	if (check_par_syntax(&input) == -1)
		return (-1);
	split_lst_contents(&input);
	if (check_redirections(&input) == -1)
		return (-1);
	clean_token_lst(&input);
	join_cmd_args(&input);
	tree = build_ast(&input, &insert_node);
	if (tree && check_tree_syntax(&tree) == -1)
        return (-1);
	ft_start_execution(&tree);
	return (0);
}

void	ft_exit(void)
{
	gbg_coll(NULL, ALL, FLUSH_ALL);
	exit(0);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*data;

	data = ft_shell();
	((void)argc, (void)argv);
	init_data(data, env);
	while (1)
	{
		data->prompt = readline("./minishell$ ");
		if (data->prompt || *data->prompt)
        {
		    start_parsing(data->prompt);
            gbg_coll(NULL, PARSING, FLUSH_ONE);
		    free(data->prompt);
        }
	}
	free(data->prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
