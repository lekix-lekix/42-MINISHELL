/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/14 15:18:03 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	check_syntax_errors(char *str);

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

int	ft_strlen_sep(char *str, char *sep)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != *sep)
		i++;
	return (i + 1);
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

int	bash_syntax_error(char *error)
{
	char	*str;

	str = malloc(sizeof(char) * (27 + ft_strlen(error)));
	if (!str)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	return (0);
}

int	start_parsing(char *prompt)
{
	int		i;
	char	error;

	i = 0;
	error = check_syntax_errors(prompt);
	if (error)
		printf("bash: syntax error near unexpected token `%c'\n", error);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	char	*path;
	char	*prompt;
	t_env	*lst;

	(void)argc;
	(void)argv;
	path = get_path(env);
	lst = get_env_lst(env);
	while (1)
	{
		prompt = readline("./minishell$");
		start_parsing(prompt);
	}
	gbg_coll(NULL, ENV, FLUSH_ONE);
	gbg_coll(NULL, ALL, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}

int	check_quotes_nb(char *str, char to_check)
{
	int	quote;
	int	i;

	quote = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == to_check)
			quote++;
	}
	if (quote % 2 == 0)
		return (0);
	return (1);
}

int	check_parenthesis_before(char *str, int i)
{
	while (i >= 0)
	{
		if (str[i] == '(')
			return (1);
		i--;
	}
	return (0);
}

char	check_syntax_errors(char *str)
{
	int	x;
	int	parenthesis;

	x = -1;
	parenthesis = 0;
	while (str[++x])
	{
		if (str[x] == '\'' && check_quotes_nb(str, '\''))
			return ('\'');
		if (str[x] == '\"' && check_quotes_nb(str, '\"'))
			return ('\"');
		if (str[x] == '(')
		{
			parenthesis++;
			if (!ft_strchr(str + x + 1, ')')) // need to check parenthesis nb
				return ('(');
		}
		if (str[x] == ')')
		{
			parenthesis--;
			if (!check_parenthesis_before(str, x))
				// need to check parenthesis nb
				return (')');
		}
	}
	if (parenthesis > 0)
		return ('(');
	else if (parenthesis < 0)
		return (')');
	return (0);
}
