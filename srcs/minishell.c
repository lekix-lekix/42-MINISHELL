/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/14 18:33:38 by kipouliq         ###   ########.fr       */
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
	// node->type = get_token_type(node->content);
	node->left = NULL;
	node->right = NULL;
	*input += operator_len;
	return (node);
}

void    parse_insert_cmd(t_token *root, t_token *node)
{
    if (root->left)
        parse_insert_cmd(root->left, node);
    if (!root->left)
    {
        root->left = node;
        return ;
    }
    if (root->right)
        parse_insert_cmd(root->right, node);
    if (!root->right)
    {
        root->right = node;
        return ;
    }
}

void    insert_cmd_node(t_token **tree, t_token *node)
{
    t_token *root;
    // t_token *last_node;

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

    root = *tree;
    if (!root)
    {
        *tree = node;
        return ;
    }
    next = root;
    *tree = node;
    node->left = next;
}

t_token	*create_cmd_node(char *input, char *sep)
{
	t_token	*node;
	int		cmd_len;

	cmd_len = ft_strlen_sep(input, sep);
	node = malloc(sizeof(t_token));
	if (!node || gbg_coll(node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	node->content = malloc(sizeof(char) * (cmd_len + 1));
	if (!node->content || gbg_coll(node->content, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(node->content, input, cmd_len + 1);
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
			insert_cmd_node(&root, cmd_node);
            input_parse = NULL;
			break ;
		}
		cmd_node = create_cmd_node(input_parse, operator);
        printf("cmd node = %s\n", cmd_node->content);
		op_node = create_operator_node(&operator);
        printf("op node = %s\n", op_node->content);
		insert_operator_node(&root, op_node);
		insert_cmd_node(&root, cmd_node);
		input_parse = operator;
	}
	return (root);
}

void    traverse_print(t_token *node)
{
    printf("%s\n", node->content);
    if (node->left)
        traverse_print(node->left);
    if (node->right)
        traverse_print(node->right);
}

void    print_tree(t_token **tree)
{
    t_token *node;

    node = *tree;
    // printf("print tree initial item = %d\n", *(int *)node->item);
    // printf("initial left = %d\n", *(int *)node->left->item);
    traverse_print(node);
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
	char	*path;
	char	*prompt;
	t_env	*lst;

	(void)argc;
	(void)argv;
	path = get_path(env);
	lst = get_env_lst(env);
	while (1)
	{
		prompt = readline("./minishell$ ");
		start_parsing(prompt);
		free(prompt);
	}
	gbg_coll(NULL, ENV, FLUSH_ONE);
	gbg_coll(NULL, ALL, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
