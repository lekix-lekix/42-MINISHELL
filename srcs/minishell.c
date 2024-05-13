/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/13 18:08:48 by kipouliq         ###   ########.fr       */
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

int check_syntax_errors(char *str)
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

int is_an_operator(char c)
{
    int i;

    i = -1;
    if (c == '|' || c == '&' || c == '(' || c == ')') // need to add && ||
            return (1);
    return (0);
}

t_token *create_cmd_node(char *input, int op_idx)
{
    t_token *node;

    node = malloc(sizeof(t_token));
    if (!node || gbg_coll(node, PARSING, ADD))
        return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
    node->content = malloc(sizeof(char) * (op_idx + 1));
    if (!node->content || gbg_coll(node->content, PARSING, ADD))
        return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
    ft_strlcpy(node->content, input, op_idx + 1);
    node->type = CMD;
    node->left = NULL;
    node->right = NULL;
    return (node);
}

t_token *tokenize_input(char *prompt)
{
    t_token *root;
    t_token *node;
    int i;

    i = 0;
    root = NULL;
    while (prompt[i])
    {
        if (is_an_operator(prompt[i]))
        {
            create_operator_node(prompt + i);
            node = create_cmd_node(prompt, i);
            if (!node || gbg_coll(node, PARSING, ADD))
                return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
            // insert_token_node(node, root);
            printf("node->content = %s\n", node->content);
        }
        i++;
    }
    return (root);
}

int start_parsing(char *prompt)
{
    t_token     *input;
    
    if (check_syntax_errors(prompt))
        return (-1);
    input = tokenize_input(prompt);
        return (-1);
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
        tokenize_input(prompt);
        free(prompt);
	}
	gbg_coll(NULL, ENV, FLUSH_ONE);
	gbg_coll(NULL, ALL, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
