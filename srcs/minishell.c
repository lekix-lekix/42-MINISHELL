/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/28 17:00:35 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	if (!data->path)
		return (-1);
	data->env_lst = get_env_lst(envp);
	if (!data->env_lst)
		return (-1);
	return (0);
}

int	print_env(t_env **lst)
{
	t_env	*node;

	node = *lst;
	while (node)
	{
		printf("field = %s\n", node->field);
		printf("content = %s\n", node->content);
		node = node->next;
	}
	return (0);
}

int	print_real_env(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	return (0);
}

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

void	print_lst(t_token **lst)
{
	t_token	*root;

	root = *lst;
	while (root)
	{
		printf("content = '%s'\n", root->content);
		printf("type = %u\n", root->type);
		root = root->next;
	}
}

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

int	start_parsing(char *prompt)
{
	t_token	*input;
	t_ast	*tree;

	if (check_syntax_errors(prompt))
		return (-1);
	input = tokenize_input(prompt);
	// print_lst(&input);
    // printf("------\n");
	clean_lst(&input);
	print_lst(&input);
	tree = build_ast(&input);
	print_tree(&tree);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*prompt;

	// char	*path;
	// t_env	*lst;
	(void)argc;
	(void)argv;
	(void)env;
	// path = get_path(env);
	// lst = get_env_lst(env);
	while (1)
	{
		prompt = readline("./minishell$ ");
		if (!prompt | !*prompt)
			break ;
		start_parsing(prompt);
		free(prompt);
	}
	free(prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
