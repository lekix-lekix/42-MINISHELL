/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/05 12:44:38 by lekix            ###   ########.fr       */
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

int	only_spaces(char *str)
{
	int	i;

	i = 0;
	while (ft_is_space(str[i]))
		i++;
	if (!str[i])
		return (1);
	return (0);
}

void	clean_lst(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	if (!*lst)
		return ;
	current = *lst;
	if (!ft_strlen(current->content) || only_spaces(current->content))
	{
		prev = current->next;
		gbg_coll(current->content, PARSING, FREE);
		gbg_coll(current, PARSING, FREE);
		*lst = prev;
	}
	current = *lst;
	while (current)
	{
		if (!ft_strlen(current->content) || only_spaces(current->content))
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
	int		par;

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
	if (par < 0)
		return (printf("syntax_error : '('\n"), -1);
	if (par > 0)
		return (printf("syntax_error : ')'\n"), -1);
	return (0);
}

void	parse_tree_size(t_ast *root, int *size)
{
	if (root->left)
		parse_tree_size(root->left, size);
	*size += 1;
	if (root->right)
		parse_tree_size(root->right, size);
}

t_ast	*get_first_node_tree(t_ast *root)
{
	if (root->left)
		return (get_first_node_tree(root->left));
	return (root);
	if (root->right)
		return (get_first_node_tree(root->right));
}

void	get_last_node_tree(t_ast *root, t_ast **node)
{
	if (root->left)
		get_last_node_tree(root->left, node);
	*node = root;
	if (root->right)
		get_last_node_tree(root->right, node);
}

int	tree_size(t_ast **tree)
{
	t_ast	*root;
	int		size;

	root = *tree;
	size = 0;
	parse_tree_size(root, &size);
	return (size);
}

void	check_tree_syntax_recursive(t_ast *root, t_ast **syntax_node,
		int *syntax_flag, int *node_nb)
{
	if (root->left)
		check_tree_syntax_recursive(root->left, syntax_node, syntax_flag,
			node_nb);
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
		check_tree_syntax_recursive(root->right, syntax_node, syntax_flag,
			node_nb);
}

int	check_tree_syntax(t_ast **tree)
{
	t_ast	*root;
	t_ast	*error_node;
	int		syntax_flag;
	int		node_nb;

	root = *tree;
	syntax_flag = 0;
	node_nb = 0;
	error_node = NULL;
	error_node = get_first_node_tree(root);
	if (error_node->node_type != CMD)
	{
		if (!error_node->is_in_par)
			return (printf("syntax error 1: %s\n",
					error_node->token_node->content), -1);
		else
			return (-1);
	}
	check_tree_syntax_recursive(root, &error_node, &syntax_flag, &node_nb);
	// printf("syntax flag = %d\n", syntax_flag);
	error_node = NULL;
	if (error_node)
	{
		if (!error_node->is_in_par)
			return (printf("syntax error 2: %s\n",
					error_node->token_node->content), -1);
		else
			return (-1);
	}
	get_last_node_tree(root, &error_node);
	if (error_node->node_type != CMD)
	{
		if (!error_node->is_in_par)
			return (printf("syntax error 3: %s\n",
					error_node->token_node->content), -1);
		else
			return (-1);
	}
	printf("syntax flag after func = %d\n", syntax_flag);
	return (0);
}

int	check_op_len(char *str, int *op_len)
{
	int	i;

	i = 0;
	while (str[i] == str[0])
		i++;
	if (i > 2)
		return (0);
	*op_len = i;
	return (1);
}

int is_redir(char c)
{
    return (c == '<' || c == '>');
}

int	check_set_redir(t_token *node)
{
	int	i;
	int	op_len;

	i = 0;
	while (node->content[i])
	{
		if ((node->content[i] == '>' || node->content[i] == '<'))
		{
			if (!check_op_len(node->content + i, &op_len))
				return (-1);
			if (node->content[i] == '<' && op_len == 1)
				node->redir = INPUT;
			if (node->content[i] == '<' && op_len == 2)
				node->redir = HEREDOC;
			if (node->content[i] == '>' && op_len == 1)
				node->redir = OUTPUT;
			if (node->content[i] == '>' && op_len == 2)
				node->redir = OUTPUT_APPEND;
			i += op_len;
			continue ;
		}
		i++;
	}
	return (0);
}

int ft_strlen_operator(char *str)
{
    int i;

    i = 0;
    while (str[i] && !is_an_operator(str[i]) && !ft_is_space(str[i]))
        i++;
    return (i);
}

void    replace_redir_blank(t_token *node, char *redir)
{
    int i;
    int j;
 
    i = 0;
    j = 0;
    printf("redir = %s\n", redir);
    while (node->content[i] && (node->content[i] != redir[0]))
        i++;
    while (node->content[i] && (node->content[i] == redir[j]))
    {
        node->content[i] = ' ';
        i++;
        j++;
    }
}

void    rem_redir_from_content(t_token *node, char *redir)
{
    replace_redir_blank(node, redir);
    printf("content after blanking = %s\n", node->content);    
}

int set_filename_token(t_token *node)
{
    char *str;
    char *redir;
    int len;
    int i;

    i = 0;
    while (!is_redir(node->content[i]))
        i++;
    redir = node->content + i;
    while (is_redir(node->content[i]))
        i++;
    str = skip_spaces(node->content + i);
    len = ft_strlen_operator(str) + 1;
    node->filename = malloc(sizeof(char) * len);
    if (!node->filename || gbg_coll(node->filename, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
    ft_strlcpy(node->filename, str, len);
    printf("str = %s\n", str);
    rem_redir_from_content(node, redir);
    return (0);
}

int	check_redirections(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return (-1);
	while (current)
	{
        printf("current->content = %s\n", current->content);
		if (current->type == CMD)
        {
			check_set_redir(current);
            if (current->redir)
                set_filename_token(current);
            printf("current->content redir = %d\n", current->redir);
            printf("current->filename = '%s'\n", current->filename);
        }
		current = current->next;
	}
    return (0);
}

int	start_parsing(char *prompt)
{
	t_token	*input;
	t_ast	*tree;
	int		insert_node;

	insert_node = 1;
	if (check_syntax_errors(prompt))
		return (-1);
	input = tokenize_input(prompt);
	if (check_par_syntax(&input) == -1)
		return (-1);
	if (check_redirections(&input) == -1)
		return (-1);
	// print_lst(&input);
	// printf("------\n")
	clean_lst(&input);
	print_lst(&input);
	tree = build_ast(&input, &insert_node);
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
