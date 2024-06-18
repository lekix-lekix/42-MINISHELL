
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_commented_out.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:46:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/18 14:50:29 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// void	insert_operator_node(t_token **tree, t_token *node)
// {
// 	t_token	*root;
// 	t_token	*next;

// 	root = *tree;
// 	if (!root)
// 	{
// 		*tree = node;
// 		return ;
// 	}
// 	next = root;
// 	*tree = node;
// 	if (!node->left)
// 	{
// 		node->left = next;
// 		return ;
// 	}
// 	if (!node->right)
// 	{
// 		node->right = next;
// 		return ;
// 	}
// }

// int	parse_insert_cmd(t_token *root, t_token *node)
// {
// 	if (root->left)
// 	{
// 		if (parse_insert_cmd(root->left, node) == 0)
// 			return (0);
// 	}
// 	if (root->type == OPERATOR && !root->left)
// 	{
// 		root->left = node;
// 		return (0);
// 	}
// 	if (root->right)
// 	{
// 		if (parse_insert_cmd(root->right, node) == 0)
// 			return (0);
// 	}
// 	if (root->type == OPERATOR && !root->right)
// 	{
// 		root->right = node;
// 		return (0);
// 	}
// 	return (1);
// }

// void	insert_cmd_node(t_token **tree, t_token *node)
// {
// 	t_token	*root;

// 	root = *tree;
// 	if (!root)
// 	{
// 		*tree = node;
// 		return ;
// 	}
// 	parse_insert_cmd(root, node);
// }

// int	parse_insert_token(t_ast *root, t_ast *node)
// {
// 	if (root->left)
// 	{
// 		if (parse_insert_token(root->left, node) == 0)
// 			return (0);
// 	}
//     if (root->node_type > node->node_type)
//     {
//         if (!root->left)
//         {
//             root->left = node;
//             return (0);
//         }
//         if (!root->right)
//         {
//             root->right = node;
//             return (0);
//         }
//     }
// 	if (root->node_type < node->node_type)
// 		return (1);
// 	if (root->right)
// 	{
// 		if (parse_insert_token(root->right, node) == 0)
// 			return (0);
// 	}
// 	return (-1);
// }

// void	insert_token_node(t_ast **tree, t_ast *node)
// {
// 	t_ast	*root;
// 	t_ast	*prev;

// 	root = *tree;
// 	if (!root)
// 	{
// 		*tree = node;
// 		return ;
// 	}
// 	if (parse_insert_token(root, node) == 1)
// 	{
// 		prev = *tree;
// 		*tree = node;
// 		if (!node->left)
// 			node->left = prev;
// 	}
// }

// t_ast	*build_ast(t_token **lst)
// {
// 	t_ast	*tree;
// 	t_token	*current;
// 	t_ast	*new_node;

// 	current = *lst;
// 	tree = NULL;
// 	if (!current)
// 		return (NULL);
// 	while (current)
// 	{
// 		printf("currnet = %s\n", current->content);
// 		new_node = malloc(sizeof(t_ast));
// 		if (!new_node || gbg_coll(new_node, PARSING, ADD))
// 			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
// 		new_node->token_node = current;
// 		if (current->type == CMD)
// 			new_node->node_type = COMMAND;
// 		else
// 			new_node->node_type = OPERATOR;
// 		new_node->left = NULL;
// 		new_node->right = NULL;
// 		insert_token_node(&tree, new_node);
// 		current = current->next;
// 	}
// 	return (tree);
// }

// void	traverse_print(t_ast *node, int level)
// {	
//     if (!node) 
//         return;
//     if (node->left)
//         traverse_print(node->left, level + 1);
//     // ft_start_exec(&node);
//     printf("The cmd is: %s\n", node->token_node->content);
//     // printf("node content = %s, level = %d\n", node->token_node->content, level);
//     if (node->right)
//         traverse_print(node->right, level + 1);
// }

// void	print_tree(t_ast **tree)
// {
// 	t_ast	*node;
	
// 	node = *tree;
// 	traverse_print(node, 0);
// }


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

// int	print_real_env(char **env)
// {
// 	int	i;

// 	i = -1;
// 	while (env[++i])
// 		printf("%s\n", env[i]);
// 	return (0);
// }

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
// }
