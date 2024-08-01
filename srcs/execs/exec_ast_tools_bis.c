/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_tools_bis.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:10:07 by lekix             #+#    #+#             */
/*   Updated: 2024/08/01 17:14:53 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*get_after_par_node(t_ast **lst)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		if (current->node_type == CMD && current->next
			&& current->next->node_type == CMD)
			return (current->next);
		current = current->next;
	}
	return (NULL);
}

t_ast	*ast_lst_dup(t_ast **lst, t_ast *node)
{
	t_ast	*lst_cpy;
	t_ast	*current_cpy;
	t_ast	*current;

	current = *lst;
	lst_cpy = NULL;
	while (current && current != node)
	{
		current_cpy = malloc(sizeof(t_token));
		if (!current_cpy || gbg_coll(current_cpy, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		current_cpy->token_node = current->token_node;
		current_cpy->node_type = current->node_type;
		current_cpy->redirections = current->redirections;
		current_cpy->is_in_par = current->is_in_par;
		current_cpy->left = current->left;
		current_cpy->right = current->right;
		current_cpy->next = NULL;
		add_ast_lst(&lst_cpy, current_cpy);
		current = current->next;
	}
	return (lst_cpy);
}

t_ast	*ast_find_one(t_ast **lst, t_ast *node)
{
	t_ast	*current;

	current = *lst;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->token_node == node->token_node)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	set_is_in_par_lst(t_ast **lst, t_ast *node, int in_par)
{
	t_ast	*current;

	current = *lst;
	if (!current)
		return ;
	if (!node)
	{
		while (current)
		{
			current->is_in_par = in_par;
			current = current->next;
		}
		return ;
	}
	while (current && current->token_node != node->token_node)
	{
		current->is_in_par = in_par;
		current = current->next;
	}
}

int	ast_list_size(t_ast **lst)
{
	t_ast	*current;
	int		i;

	current = *lst;
	i = 0;
	while (current)
	{
		current = current->next;
		i++;
	}
	return (i);
}
