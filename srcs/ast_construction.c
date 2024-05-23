/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_construction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:48:33 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/23 19:18:59 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	traverse_print(t_ast *node, int level)
{
	if (node->left)
		traverse_print(node->left, level + 1);
	printf("node content = %s, level = %d\n", node->token_node->content, level);
	if (node->right)
		traverse_print(node->right, level + 1);
}

void	print_tree(t_ast **tree)
{
	t_ast	*node;

	node = *tree;
	traverse_print(node, 0);
}

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

int	parse_insert_op_token_node(t_ast *root, t_ast *node, int level)
{
	// printf("node = %s\n level = %d\n", node->token_node->content, level);
	if (root->left)
		parse_insert_op_token_node(root->left, node, level + 1);
	if (root->right)
		parse_insert_op_token_node(root->right, node, level + 1);
    // printf("root = %s type = %d, node = %s type = %d\n", root->token_node->content, root->node_type, node->token_node->content, node->node_type);
	if (root->node_type > node->node_type)
	{
		if (!root->left)
			root->left = node;
		else
			root->right = node;
		return (0);
	}
	if (root->node_type <= node->node_type && level == 0)
		return (1);
	// add error case
	return (-1);
}

void	insert_operator_token_node(t_ast **tree, t_ast *node)
{
	t_ast	*root;
    int     return_value;

	root = *tree;
	if (!root)
	{
		*tree = node;
		return ;
	}
	return_value = parse_insert_op_token_node(root, node, 0);
    if (return_value == 1)
	{
		*tree = node;
		node->left = root;
	}
}

t_token	*find_operator_token(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->type > 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_ast	*create_ast_node(t_token *node)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node || gbg_coll(new_node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	new_node->token_node = node;
	new_node->left = NULL;
	new_node->right = NULL;
    new_node->node_type = node->type;
	return (new_node);
}

void    consume_node(t_token **lst, t_token *node)
{
    t_token *current;
    t_token *prev;

    current = *lst;
    if (!current || !node)
        return ;
    while (current)
    {
        // printf("current = %s\n", current->content);
        if (current == node)
        {
            prev->next = current->next;
            gbg_coll(node->content, PARSING, FREE);
            // gbg_coll(node, PARSING, FREE); // bizarre ? probable leak
        }
        prev = current;
        current = current->next;
    }
}

t_ast	*build_operator_tree(t_token **lst)
{
	t_ast	*tree;
    t_ast   *right;
	t_token	*current;
	t_ast	*new_node;

	tree = NULL;
    right = NULL;
	while (1)
	{
		current = find_operator_token(lst);
		if (!current)
			break ;
		new_node = create_ast_node(current);
        consume_node(lst, new_node->token_node);
        if (tree && new_node->node_type < tree->node_type)
            insert_operator_token_node(&right, new_node);
        else
            insert_operator_token_node(&tree, new_node);
	}
    tree->right = right;
	return (tree);
}

int    parse_insert_cmd_node(t_ast *root, t_ast *cmd_node, int level)
{
    // printf("root = %s, cmd-node = %s level = %d\n", root->token_node->content, cmd_node->token_node->content, level);
    if (root->left)
    {
        if (parse_insert_cmd_node(root->left, cmd_node, level + 1) == 0)
            return (0);
    }
    if (!root->left && root->node_type > cmd_node->node_type)
    {
        root->left = cmd_node;
        return (0);
    }
    if (root->right)
    {
        if (parse_insert_cmd_node(root->right, cmd_node, level + 1) == 0)
            return (0);
    }
    if (!root->right && root->node_type > cmd_node->node_type)
    {
        root->right = cmd_node;
        return (0);
    }
    return (-1);
}

void    insert_command_node(t_ast **tree, t_token *cmd_node)
{
    t_ast *root;
    t_ast *new_cmd_node;

    root = *tree;
    new_cmd_node = create_ast_node(cmd_node);
    parse_insert_cmd_node(root, new_cmd_node, 0);
    // printf("insert cmd = %d\n", parse_insert_cmd_node(root, new_cmd_node, 0));
}

void    build_cmd_tree(t_ast **tree, t_token **lst)
{
    t_token *current;

    current = *lst;
    while (current)
    {
        insert_command_node(tree, current);
        current = current->next;
    }
}

t_ast	*build_ast(t_token **lst)
{
	t_ast	*tree;

	tree = build_operator_tree(lst);
    build_cmd_tree(&tree, lst);
	return (tree);
}

// cmd1 | cmd2 || cmd3 | cmd4