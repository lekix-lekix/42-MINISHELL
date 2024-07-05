/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_construction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:48:33 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/05 16:30:11 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	consume_node(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	if (!current || !node)
		return ;
	if (current == node)
	{
		*lst = current->next;
		return ;
	}
	while (current)
	{
		if (current == node)
			prev->next = current->next;
		prev = current;
		current = current->next;
	}
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
			current = find_closing_par(&current);
			continue ;
		}
		if (!current)
			break ;
		create_consume_insert_node(lst, &current, &tree, &right);
		if (current)
			current = current->next;
	}
	if (tree && right)
		tree->right = right;
	return (tree);
}

int	create_insert_ast_node_par(t_ast **tree, t_token *current)
{
	t_ast	*cmd_node;
	int		insert_node;

	insert_node = 1;
	cmd_node = handle_par(&current, tree, &insert_node);
	if (!cmd_node)
	{
		*tree = NULL;
		return (-1);
	}
	insert_cmd_node(tree, cmd_node);
	return (0);
}

int	build_cmd_tree(t_ast **tree, t_token **lst)
{
	t_token	*current;
	t_ast	*cmd_node;

	current = *lst;
	if (!current)
		return (-1);
	while (current)
	{
		if (current->type == PAR_LEFT)
		{
			if (create_insert_ast_node_par(tree, current) == -1)
				return (-1);
			current = find_closing_par(&current);
			continue ;
		}
		else
		{
			cmd_node = create_ast_node(current);
			insert_cmd_node(tree, cmd_node);
		}
		if (current)
			current = current->next;
	}
	return (0);
}

t_ast	*build_ast(t_token **lst, int *insert_node)
{
	t_ast	*tree;

	tree = build_operator_tree(lst);
	if (build_cmd_tree(&tree, lst) == -1)
		*insert_node = 0;
	return (tree);
}
