/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_insert_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:17:54 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/28 17:06:37 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	parse_insert_op_token_node(t_ast *root, t_ast *node, int level)
{
	if (root->left)
		parse_insert_op_token_node(root->left, node, level + 1);
	if (root->right)
		parse_insert_op_token_node(root->right, node, level + 1);
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
	return (-1);
}

void	insert_operator_token_node(t_ast **tree, t_ast *node)
{
	t_ast	*root;

	root = *tree;
	if (!root)
	{
		*tree = node;
		return ;
	}
	if (parse_insert_op_token_node(root, node, 0) == 1)
	{
		*tree = node;
		node->left = root;
	}
}

int	parse_insert_cmd_node(t_ast *root, t_ast *cmd_node, int level)
{
	if (root->left)
	{
		if (parse_insert_cmd_node(root->left, cmd_node, level + 1) == 0)
			return (0);
	}
	if (!root->left && (root->node_type > cmd_node->node_type
			|| (root->node_type > CMD && cmd_node->is_in_par)))
	{
		root->left = cmd_node;
		return (0);
	}
	if (root->right)
	{
		if (parse_insert_cmd_node(root->right, cmd_node, level + 1) == 0)
			return (0);
	}
	if (!root->right && (root->node_type > cmd_node->node_type
			|| (root->node_type > CMD && cmd_node->is_in_par)))
	{
		root->right = cmd_node;
		return (0);
	}
	return (-1);
}

int	insert_cmd_node(t_ast **tree, t_ast *node)
{
	t_ast	*root;

	root = *tree;
	if (!root)
	{
		*tree = node;
		return (0);
	}
	if (parse_insert_cmd_node(root, node, 0) == -1)
		return (-1);
	return (0);
}

void	create_consume_insert_node(t_token **lst, t_token **node, t_ast **tree,
		t_ast **tree_right)
{
	t_ast	*new_node;

	new_node = create_ast_node(*node);
	if (*lst == *node)
	{
		consume_node(lst, new_node->token_node);
		*node = NULL;
	}
	consume_node(lst, new_node->token_node);
	if (*tree_right)
		insert_operator_token_node(tree_right, new_node);
	else if (*tree && new_node->node_type < (*tree)->node_type)
		insert_operator_token_node(tree_right, new_node);
	else
		insert_operator_token_node(tree, new_node);
}
