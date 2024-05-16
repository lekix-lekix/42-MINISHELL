/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_construction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:48:33 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/16 18:36:09 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	traverse_print(t_token *node, int level)
{
	if (node->left)
		traverse_print(node->left, level + 1);
	printf("node content = %s, type = %d\n", node->content, node->type);
	if (node->right)
		traverse_print(node->right, level + 1);
}

void	print_tree(t_token **tree)
{
	t_token	*node;

	node = *tree;
	traverse_print(node, 0);
}

void	insert_operator_node(t_token **tree, t_token *node)
{
	t_token	*root;
	t_token	*next;

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

int	parse_insert_cmd(t_token *root, t_token *node)
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

void	insert_cmd_node(t_token **tree, t_token *node)
{
	t_token	*root;

	root = *tree;
	if (!root)
	{
		*tree = node;
		return ;
	}
	parse_insert_cmd(root, node);
}
