/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/31 16:48:22 by kipouliq         ###   ########.fr       */
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

int	check_par_syntax(t_token **lst)
{
	t_token	*current;
	size_t	par;

	current = *lst;
	if (!current)
		return (-1);
	par = 0;
	while (current)
	{
		if (current->type == PAR_LEFT)
			par--;
		else if (current->type == PAR_RIGHT)
			par++;
		current = current->next;
	}
	if (par > 0)
		return (printf("syntax_error : '('\n"), -1);
	if (par < 0)
		return (printf("syntax_error : ')'\n"), -1);
	return (0);
}

void    parse_tree_size(t_ast *root, int *size)
{
    if (root->left)
        parse_tree_size(root->left, size);
    *size += 1;
    if (root->right)
        parse_tree_size(root->right, size);
}

t_ast *get_first_node_tree(t_ast *root)
{
    if (root->left)
        return (get_first_node_tree(root->left));
    return (root);
    if (root->right)
        return (get_first_node_tree(root->right));
}

void    get_last_node_tree(t_ast *root, t_ast **node)
{
    if (root->left)
        get_last_node_tree(root->left, node);
    *node = root;
    if (root->right)
        get_last_node_tree(root->right, node);
}

int tree_size(t_ast **tree)
{
    t_ast *root;
    int size;

    root = *tree;
    size = 0;
    parse_tree_size(root, &size);
    return (size);
}

void check_tree_syntax_recursive(t_ast *root, t_ast **syntax_node, int *syntax_flag, int *node_nb)
{
    if (root->left)
        check_tree_syntax_recursive(root->left, syntax_node, syntax_flag, node_nb);
    if (root->node_type == CMD)
        *syntax_flag += 1;
    if (root->node_type > CMD)
        *syntax_flag -= 1;
    if (*syntax_flag > 1 || *syntax_flag < 0)
    {
        *syntax_node = root;
        return ;
    }
    *node_nb += 1;
    if (root->right)
        check_tree_syntax_recursive(root->right, syntax_node, syntax_flag, node_nb);
}

int check_tree_syntax(t_ast **tree)
{
    t_ast *root;
    t_ast  *error_node;
    int    syntax_flag;
    int    node_nb;

    root = *tree;
    syntax_flag = 0;
    node_nb = 0;
    error_node = NULL;
    error_node = get_first_node_tree(root);
    if (error_node->node_type != CMD)
        return (printf("syntax error 1: %s\n", error_node->token_node->content), -1);
    check_tree_syntax_recursive(root, &error_node, &syntax_flag, &node_nb);
    printf("syntax flag = %d\n", syntax_flag);
    error_node = NULL;
    if (error_node)
        return (printf("syntax error 2: %s\n", error_node->token_node->content), -1);
    get_last_node_tree(root, &error_node);
    if (error_node->node_type != CMD)
        return (printf("syntax error 3: %s\n", error_node->token_node->content), -1);
    printf("syntax flag after func = %d\n", syntax_flag);
    return (0);
}

int	start_parsing(char *prompt)
{
	t_token	*input;
	t_ast	*tree;

	if (check_syntax_errors(prompt))
		return (-1);
	input = tokenize_input(prompt);
	if (check_par_syntax(&input) == -1)
		return (-1);
	// print_lst(&input);
	// printf("------\n");
	clean_lst(&input);
	print_lst(&input);
	tree = build_ast(&input);
	if (tree)
    {
        printf("on veut print\n");
		print_tree(&tree);
        check_tree_syntax(&tree);
    }
    
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
