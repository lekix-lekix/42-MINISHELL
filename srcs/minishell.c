/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/06 18:33:06 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*msh_strdup(const char *s, int mlc_lst)
{
	char	*final_str;
	int		s_len;

	s_len = ft_strlen(s);
	final_str = malloc(sizeof(char) * (s_len + 1));
	if (!final_str || gbg_coll(final_str, mlc_lst, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(final_str, s, s_len + 1);
	return (final_str);
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
		printf("--------\n");
		printf("content = '%s'\n", root->content);
		printf("type = %u\n", root->type);
		// printf("redir = %d\n", root->redir);
		printf("filename = '%s'\n", root->filename);
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

void	gbg_delete_lst(t_token *node, int mlc_lst)
{
	gbg_coll(node->content, mlc_lst, FREE);
	gbg_coll(node, mlc_lst, FREE);
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
		gbg_delete_lst(current, PARSING);
		*lst = prev;
	}
	current = *lst;
	while (current)
	{
		if (!ft_strlen(current->content) || only_spaces(current->content))
		{
			prev->next = current->next;
			gbg_delete_lst(current, PARSING);
		}
		prev = current;
		current = current->next;
	}
}

void	remove_token_node(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*prev;

	if (!*lst)
		return ;
	current = *lst;
	if (current == node)
	{
		prev = current->next;
		gbg_delete_lst(current, PARSING);
		*lst = prev;
		return ;
	}
	current = *lst;
	while (current)
	{
		if (current == node)
		{
			prev->next = current->next;
			gbg_delete_lst(current, PARSING);
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

int	is_redir_operator(char c)
{
	return (c == '<' || c == '>');
}

// int	check_set_redir(t_token *node)
// {
// 	int	i;
// 	int	op_len;

// 	i = 0;
// 	while (node->content[i])
// 	{
// 		if ((node->content[i] == '>' || node->content[i] == '<'))
// 		{
// 			if (!check_op_len(node->content + i, &op_len))
// 				return (-1);
// 			if (node->content[i] == '<' && op_len == 1)
// 				node->redirections = INPUT;
// 			if (node->content[i] == '<' && op_len == 2)
// 				node->redirections = HEREDOC;
// 			if (node->content[i] == '>' && op_len == 1)
// 				node->redirections = OUTPUT;
// 			if (node->content[i] == '>' && op_len == 2)
// 				node->redirections = OUTPUT_APPEND;
// 			i += op_len;
// 			continue ;
// 		}
// 		i++;
// 	}
// 	return (0);
// }

int	ft_strlen_operator(char *str)
{
	int	i;

	i = 0;
	while (str[i] && !is_an_operator(str[i]) && !ft_is_space(str[i]))
		i++;
	return (i);
}

void	replace_redir_blank(t_token *node, char *redir)
{
	int	i;

	i = 0;
	while (node->content[i] && (node->content[i] != redir[0]))
		i++;
	while (node->content[i] && is_redir_operator(node->content[i]))
	{
		node->content[i] = ' ';
		i++;
	}
	while (node->content[i] && ft_is_space(node->content[i]))
		i++;
	while (node->content[i] && !ft_is_space(node->content[i]))
	{
		node->content[i] = ' ';
		i++;
	}
}

int	set_filename_token(t_token *node)
{
	char	*str;
	char	*redir;
	int		len;
	int		i;

	i = 0;
	while (!is_redir_operator(node->content[i]))
		i++;
	redir = node->content + i;
	while (is_redir_operator(node->content[i]))
		i++;
	str = skip_spaces(node->content + i);
	len = ft_strlen_operator(str);
	node->filename = malloc(sizeof(char) * (len + 1));
	if (!node->filename || gbg_coll(node->filename, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	ft_strlcpy(node->filename, str, len + 1);
	replace_redir_blank(node, redir);
	return (0);
}

// int	check_redirections(t_token **lst)
// {
// 	t_token	*current;

// 	current = *lst;
// 	if (!current)
// 		return (-1);
// 	while (current)
// 	{
// 		if (current->type == CMD)
//         {
// 			check_set_redir(current);
//             if (current->redir)
//                 set_filename_token(current);
//         }
// 		current = current->next;
// 	}
//     return (0);
// }

char	*get_filename(t_token *node)
{
	int		i;
	char	*filename;
	int		filename_len;

	i = 0;
	filename_len = ft_strlen_sep(node->content, " ");
	printf("filename len = %d\n", filename_len);
	filename = malloc(sizeof(char) * (filename_len + 1));
	if (!filename || gbg_coll(filename, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(filename, node->content, filename_len);
	i = -1;
	while (node->content[++i] && i < filename_len)
		node->content[i] = ' ';
	return (filename);
}

t_token	*find_redir_node(t_token **lst)
{
	t_token	*cmd_node;
	t_token	*current;
	int		found_redir;

	current = *lst;
	if (!current)
		return (NULL);
	found_redir = 0;
	cmd_node = NULL;
	while (current)
	{
        // printf("current = %s type = %d\n", current->content, current->type);
		// if (cmd_node)
		// 	printf("found cmd node = %s\n", cmd_node->content);
		if (current->type == CMD)
			cmd_node = current;
		if ((current->type > CMD && current->type < REDIR_INPUT))
		{
			printf("RETURNING\n");
			return (cmd_node);
		}
		current = current->next;
	}
	return (cmd_node);
}

void	set_redir_lst(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return ;
	while (current)
	{
		if ((current->type >= REDIR_INPUT && current->type < OUTFILE) && current->next)
			current->next->type = OUTFILE;
		current = current->next;
	}
}

int	handle_redirection(t_token **lst, t_token *redir)
{
	t_token	*redir_node;
	t_token	*filename_token;
	int		redir_type;
	char	*filename;

	printf("node = %s type = %u\n", redir->content, redir->type);
	if (!redir->next)
		return (-1); // syntax error : newline
	filename_token = redir->next;
	redir_type = redir->type;
	filename = get_filename(filename_token);
	redir_node = find_redir_node(lst);
	printf("filename = %s\n", filename);
	printf("node->content = %s\n", filename_token->content);
	printf("found cmd_node = %s\n", redir_node->content);
	return (0);
}

int	check_redirections(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return (-1);
	set_redir_lst(lst);
	while (current)
	{
		if (current->type >= REDIR_INPUT && current->type < OUTFILE)
			handle_redirection(lst, current);
		current = current->next;
	}
	return (0);
}

int	trim_token_fields(t_token **lst)
{
	t_token	*current;
	char	*str;

	current = *lst;
	while (current)
	{
		if (current->content)
		{
			str = msh_strtrim(current->content, " ");
			gbg_coll(current->content, PARSING, FREE);
			current->content = str;
		}
		if (current->filename)
		{
			str = msh_strtrim(current->filename, " ");
			gbg_coll(current->filename, PARSING, FREE);
			current->filename = str;
		}
		current = current->next;
	}
	return (0);
}

int	clean_token_lst(t_token **lst)
{
	clean_lst(lst);
	trim_token_fields(lst);
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
	clean_token_lst(&input);
	if (check_redirections(&input) == -1)
		return (-1);
	printf("TOKEN LST BEFORE AST =======\n");
	print_lst(&input);
	printf("============================\n");
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
		printf("===========\n");
	}
	free(prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
