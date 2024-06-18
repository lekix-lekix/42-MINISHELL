/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_construction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:48:33 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/18 14:37:36 by sabakar-         ###   ########.fr       */
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
	// add error case
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

void	consume_node(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	if (!current || !node)
		return ;
	while (current)
	{
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

int	parse_insert_cmd_node(t_ast *root, t_ast *cmd_node, int level)
{
	if (!root || !cmd_node)
		return (-1);
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
