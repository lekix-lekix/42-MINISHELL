/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/18 14:53:36 by sabakar-         ###   ########.fr       */
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

static void	ft_start_execution(t_ast **tree, t_minishell *data)
{
	t_ast	*nodes;
	int		la_status;

	nodes = *tree;
	la_status = ft_start_exec(&nodes, data);
}

int	start_parsing(char *prompt, t_minishell *data)
{
	t_token	*input;
	t_ast	*tree;

	if (check_syntax_errors(prompt))
		return (-1);
	input = tokenize_input(prompt);
	clean_lst(&input);
	tree = build_ast(&input);
	ft_start_execution(&tree, data);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	data;

	((void)argc, (void)argv);
	data.env_lst = get_env_lst(env);
	while (1)
	{
		data.prompt = readline("./minishell$ ");
		if (!data.prompt | !*data.prompt)
			break ;
		start_parsing(data.prompt, &data);
		free(data.prompt);
	}
	free(data.prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
