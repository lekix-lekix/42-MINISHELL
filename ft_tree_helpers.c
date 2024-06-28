/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tree_helpers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:36:08 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/18 14:44:02 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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

t_ast	*build_operator_tree(t_token **lst)
{
	t_ast	*tree;
	t_ast	*right;
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

void	insert_command_node(t_ast **tree, t_token *cmd_node)
{
	t_ast	*root;
	t_ast	*new_cmd_node;
	int		result;

	root = *tree;
	new_cmd_node = create_ast_node(cmd_node);
	if (root == NULL)
		*tree = new_cmd_node;
	else
	{
		result = parse_insert_cmd_node(root, new_cmd_node, 0);
		if (result == -1)
		{
			new_cmd_node->left = root;
			*tree = new_cmd_node;
		}
	}
}

void	build_cmd_tree(t_ast **tree, t_token **lst)
{
	t_token	*current;

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
