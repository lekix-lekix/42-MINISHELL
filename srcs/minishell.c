/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/15 17:49:05 by lekix            ###   ########.fr       */
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
	if (!sep)
		return (ft_strlen(str));
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

int	is_an_operator(char c)
{
	return (c == '|' || c == '&' || c == '(' || c == ')'); // need to add && ||
}

char	*find_operator(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (is_an_operator(str[i]))
			return (str + i);
	}
	return (NULL);
}

int	check_null_str(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (str[i] != ' ')
			return (1);
	}
	return (0);
}

t_token	*create_operator_node(char **input)
{
	t_token	*node;
	int		operator_len;
	char	*input_copy;

	input_copy = *input;
	operator_len = ft_strlen_sep(input_copy, " ");
	node = malloc(sizeof(t_token));
	if (!node || gbg_coll(node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	node->content = malloc(sizeof(char) * (operator_len + 1));
	if (!node->content || gbg_coll(node->content, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(node->content, input_copy, operator_len + 1);
	node->type = OPERATOR;
	node->left = NULL;
	node->right = NULL;
	*input += operator_len;
	return (node);
}

int parse_insert_cmd(t_token *root, t_token *node)
{
    if (root->left)
    {
        if (parse_insert_cmd(root->left, node) == 0)
            return (0);
    }
    if (root->type == OPERATOR && !root->left)
    {
        root->left = node;
        return (0);
    }
    if (root->right)
    {
        if (parse_insert_cmd(root->right, node) == 0)
            return (0);
    }
    if (root->type == OPERATOR && !root->right)
    {
        root->right = node;
        return (0);
    }
    return (1);
}

void    insert_cmd_node(t_token **tree, t_token *node)
{
    t_token *root;

    root = *tree;
    if (!root)
    {
        *tree = node;
        return ;
    }
    parse_insert_cmd(root, node);
}

void    insert_operator_node(t_token **tree, t_token *node)
{
    t_token *root;
    t_token *next;

    printf("node content add = %s\n", node->content);
    root = *tree;
    if (!root)
    {
        *tree = node;
        return ;
    }
    next = root;
    *tree = node;
    if (!node->left)
    {
        node->left = next;
        return ;
    }
    if (!node->right)
    {
        node->right = next;
        return ;
    }
}

int    ft_strcpy_sep(char *dest, char *input, char *sep)
{
    int i;

    i = -1;
    if (!sep)
        return (ft_strlcpy(dest, input, ft_strlen(input) + 1));
    while (input[++i] && input[i] != sep[0])
        dest[i] = input[i];
    dest[i] = '\0';
    return (i);
}

t_token	*create_cmd_node(char *input, char *sep)
{
	t_token	*node;
	int		cmd_len;

	cmd_len = ft_strlen_sep(input, sep);
    // printf("input = %s\n", input);
    // printf("cmd len = %d\n", cmd_len);
    // printf("sep = %s\n", sep);
	node = malloc(sizeof(t_token));
	if (!node || gbg_coll(node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	node->content = malloc(sizeof(char) * (cmd_len + 1));
	if (!node->content || gbg_coll(node->content, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strcpy_sep(node->content, input, sep);
	node->type = CMD;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_token	*tokenize_input(char *input)
{
	t_token	*root;
	t_token	*cmd_node;
	t_token	*op_node;
	char	*input_parse;
	char	*operator;

	root = NULL;
	input_parse = input;
	while (input_parse)
	{
		operator = find_operator(input_parse);
		if (!operator)
		{
			cmd_node = create_cmd_node(input_parse, NULL);
            // printf("cmd node = %s\n", cmd_node->content);
			insert_cmd_node(&root, cmd_node);
			break ;
		}
		op_node = create_operator_node(&operator);
		cmd_node = create_cmd_node(input_parse, op_node->content);
        // printf("cmd node = %s\n", cmd_node->content);
        // printf("op node = %s\n", op_node->content);
		insert_operator_node(&root, op_node);
		insert_cmd_node(&root, cmd_node);
		input_parse = operator;
	}
	return (root);
}

void    traverse_print(t_token *node, int level)
{
    printf("content = %s, type = %d level = %d\n", node->content, node->type, level);
    if (node->left)
        printf("node left = %s\n", node->left->content);
    if (node->right)
        printf("node->right = %s\n", node->right->content);
    if (node->left)
        traverse_print(node->left, level + 1);
    if (node->right)
        traverse_print(node->right, level + 1);
}

void    print_tree(t_token **tree)
{
    t_token *node;

    node = *tree;
    // printf("print tree initial item = %d\n", *(int *)node->item);
    // printf("initial left = %d\n", *(int *)node->left->item);
    traverse_print(node, 0);
}
 
int	start_parsing(char *prompt)
{
	t_token	*input;

	if (check_syntax_errors(prompt))
		return (-1);
	input = tokenize_input(prompt);
    print_tree(&input);
	return (0);
}

int	main(int argc, char **argv, char **env)
{
	// char	*path;
	char	*prompt;
	// t_env	*lst;

	(void)argc;
	(void)argv;
    (void)env;
	// path = get_path(env);
	// lst = get_env_lst(env);
	while (1)
	{
		prompt = readline("./minishell$ ");
        if (!*prompt)
            continue ;
		start_parsing(prompt);
		free(prompt);
	}
	gbg_coll(NULL, ENV, FLUSH_ONE);
	gbg_coll(NULL, ALL, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
