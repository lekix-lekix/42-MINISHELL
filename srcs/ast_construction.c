/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_construction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:48:33 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/29 18:18:12 by kipouliq         ###   ########.fr       */
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

void	create_consume_insert_node(t_token **lst, t_token *node, t_ast **tree,
		t_ast **tree_right)
{
	t_ast	*new_node;

	new_node = create_ast_node(node);
	consume_node(lst, new_node->token_node);
	if (*tree && new_node->node_type < (*tree)->node_type)
		insert_operator_token_node(tree_right, new_node);
	else
		insert_operator_token_node(tree, new_node);
}

t_ast	*build_operator_tree(t_token **lst)
{
	t_ast	*tree;
	t_ast	*right;
	t_token	*current;

	tree = NULL;
	right = NULL;
	current = *lst;
	while (current)
	{
		current = find_operator_token(&current);
		if (current && current->type == PAR_LEFT)
		{
			current = find_closing_par(&current)->next;
			continue ;
		}
		if (!current)
			break ;
		create_consume_insert_node(lst, current, &tree, &right);
		current = current->next;
	}
	if (tree && right)
		tree->right = right;
	return (tree);
}

// void	build_cmd_tree(t_ast **tree, t_token **lst)
// {
// 	t_token	*current;
// 	t_ast	*cmd_node;
// 	t_ast	*root;

// 	current = *lst;
// 	root = *tree;
// 	while (current)
// 	{
// 		if (current->type == PAR_LEFT)
// 		{
// 			cmd_node = handle_par(&current, tree, &root);
// 			if (!cmd_node) // handle unclosed par here ?
// 				return ;
//             parse_insert_cmd_node(root, cmd_node, 0);
// 			current = find_closing_par(&current);
// 			continue ;
// 		}
// 		else
// 			cmd_node = create_ast_node(current);
// 		parse_insert_cmd_node(root, cmd_node, 0);
// 		current = current->next;
// 	}
// }

void	build_cmd_tree(t_ast **tree, t_token **lst)
{
	t_token	*current;
	t_ast	*cmd_node;
	t_ast	*root;

	current = *lst;
	root = *tree;
	while (current)
	{
		if (current->type == PAR_LEFT)
		{
			cmd_node = handle_par(&current, tree, &root);
			if (!cmd_node) // handle unclosed par here ?
				return ;
            parse_insert_cmd_node(root, cmd_node, 0);
			current = find_closing_par(&current);
			continue ;
		}
		else
			cmd_node = create_ast_node(current);
		parse_insert_cmd_node(root, cmd_node, 0);
		current = current->next;
	}
}

t_ast	*build_ast(t_token **lst)
{
	t_ast	*tree;

	// tree = NULL;
	tree = build_operator_tree(lst);
	build_cmd_tree(&tree, lst);
	return (tree);
}

// cmd1 | cmd2 || cmd3 | cmd4
// (1 && 2) | (3 && 4)
// (1 && 2) || (3 && 4)
// ((1 && 2) || 4) | 4
// ((1 && 2) || (3 && 4) || 5) SIGSEV
// (((1 || 2) || 3) || 4)
