/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/15 17:07:04 by lekix            ###   ########.fr       */
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

void    traverse_count_pipes(t_ast *root, int *pipes)
{
    if (root->left)
        traverse_count_pipes(root->left, pipes);
    if (root->node_type == PIPE)
        *pipes += 1;
    if (root->right)
        traverse_count_pipes(root->right, pipes);
}

int ft_count_pipes(t_ast **tree)
{
    int pipes;

    pipes = 0;
    if (!*tree)
        return (0);
    traverse_count_pipes(*tree, &pipes);
    return (pipes);
}

int	**alloc_pipes_tab(t_ast **tree)
{
	int	**tab;
    int size;
	int	i;
	int	j;

	i = -1;
    size = ft_count_pipes(tree);
	tab = malloc(sizeof(int *) * (size + 1));
	if (!tab || gbg_coll(tab, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	while (++i < size)
	{
		tab[i] = malloc(sizeof(int) * 2);
		if (!tab[i] || gbg_coll(tab[i], PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		j = -1;
		while (++j < 2)
			tab[i][j] = 0;
	}
	tab[i] = NULL;
	return (tab);
}

static void	ft_start_execution(t_ast **tree)
{
	t_ast	*nodes;
	// int		la_status;

	nodes = *tree;
	ft_init_tree(nodes);
	// if (nodes->token_node && nodes->token_node->type == CMD)
	// {
	// 	printf("It was a single cmd\n");
	// 	la_status = ft_exec_non_builtins_single_cmd(nodes->token_node->contents, nodes->redirections);
	// }
	// else
	/* la_status =  */
	ft_shell()->pipes = alloc_pipes_tab(tree);
    ft_start_exec(&nodes);
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
    print_lst(&input);
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
    printf("LST BEFORE AST ===\n");
    print_lst(&input);
    printf("==================\n");
	tree = build_ast(&input, &insert_node);
    printf("TREE BEFORE EXEC\n");
    print_tree(&tree);
    printf("====\n");
	if (tree && check_tree_syntax(&tree) == -1)
        return (-1);
    ft_shell()->exec_tree = tree;
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
