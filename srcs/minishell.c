/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/17 15:04:13 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"
// t_minishell g_minishell;

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

// int	init_data(t_minishell *data, char **envp)
// {
// 	data->path = get_path(envp);
// 	if (!data->path)
// 		return (-1);
// 	data->env_lst = get_env_lst(envp);
// 	if (!data->env_lst)
// 		return (-1);
// 	return (0);
// }

int	print_env(t_env **lst)
{
	t_env	*node;

	node = *lst;
	while (node)
	{
		printf("%s", node->field);
		printf("%s\n", node->content);
		node = node->next;
	}
	return (0);
}

// int	print_real_env(char **env)
// {
// 	int	i;

// 	i = -1;
// 	while (env[++i])
// 		printf("%s\n", env[i]);
// 	return (0);
// }

int	check_syntax_errors(char *str)
{
	int		i;
	char	c;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			c = str[i];
			str = ft_strchr(str + i + 1, c);
			if (!str)
			{
				printf("bash: syntax error near unexpected token `%c'\n", c);
				return (-1);
			}
			i = 0;
			continue ;
		}
	}
	return (0);
}

// void	print_lst(t_token **lst)
// {
// 	t_token	*root;

// 	root = *lst;
// 	while (root)
// 	{
// 		printf("content = '%s'\n", root->content);
// 		printf("type = %u\n", root->type);
// 		root = root->next;
// 	}
// }

void	clean_lst(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	if (!*lst)
		return ;
	current = *lst;
	if (!ft_strlen(current->content))
	{
		prev = current->next;
		gbg_coll(current->content, PARSING, FREE);
		gbg_coll(current, PARSING, FREE);
		*lst = prev;
	}
	current = *lst;
	while (current)
	{
		if (!ft_strlen(current->content))
		{
			prev->next = current->next;
			gbg_coll(current->content, PARSING, FREE);
			gbg_coll(current, PARSING, FREE);
		}
		prev = current;
		current = current->next;
	}
}

static void	ft_start_execution(t_ast **tree, t_minishell *data)
{
	t_ast *nodes;
	int la_status;

	nodes = *tree;
	la_status = ft_start_exec(&nodes, data);
	// printf("The status code: %d\n", la_status);
}

int	start_parsing(char *prompt, t_minishell *data)
{
	t_token	*input;
	t_ast	*tree;
	
	if (check_syntax_errors(prompt))
		return (-1);
	input = tokenize_input(prompt);
	// print_lst(&input);
    // printf("------\n");
	clean_lst(&input);
	tree = build_ast(&input);
	// print_tree(&tree);
	ft_start_execution(&tree, data);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell data;
	
	((void)argc, (void)argv);
	data.env_lst = get_env_lst(env);
	while (1)
	{
		data.prompt = readline("./minishell$ ");
		if (!data.prompt | !*data.prompt)
			break ;
		// printf("The data.prompt: %s\n", data.prompt);
		start_parsing(data.prompt, &data);
		// ft_start_execution();
		// print_env(&lst);
		free(data.prompt);
	}
	free(data.prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
