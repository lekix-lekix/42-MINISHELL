/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_insert_node.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:17:54 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/31 18:04:04 by kipouliq         ###   ########.fr       */
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
        // printf("node nest = %s\n", node->right->token_node->content);
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
    // printf("root = %s cmd_npde = %s\n", root->token_node->content, cmd_node->token_node->content);
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

int insert_cmd_node(t_ast **tree, t_ast *node)
{
    t_ast *root;
    
    root = *tree;
    if (!root)
    {
        root = node;
        return (0);
    }
    if (parse_insert_cmd_node(root, node, 0) == -1)
        return (-1);
    return (0);
}
