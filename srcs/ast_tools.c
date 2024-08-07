/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:27:57 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/07 15:25:02 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*lst_dup(t_token **lst, t_token *node)
{
	t_token	*lst_cpy;
	t_token	*current_cpy;
	t_token	*current;

	current = *lst;
	lst_cpy = NULL;
	while (current && current != node)
	{
		current_cpy = malloc(sizeof(t_token));
		if (!current_cpy || gbg_coll(current_cpy, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		current_cpy->type = current->type;
		current_cpy->content = current->content;
		if (current->type != CMD)
			current_cpy->contents = NULL;
		else
			current_cpy->contents = current->contents;
		current_cpy->redirections = current->redirections;
		current_cpy->pipe_redir[0] = current->pipe_redir[0];
		current_cpy->pipe_redir[1] = current->pipe_redir[1];
		current_cpy->next = NULL;
		if (current->original_token)
			current_cpy->original_token = current->original_token;
		else
			current_cpy->original_token = current;
        current_cpy->is_in_par = current->is_in_par;
		insert_node_lst(&lst_cpy, current_cpy);
		current = current->next;
	}
	return (lst_cpy);
}

t_token	*find_operator_token(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->type > CMD && current->type < PAR_RIGHT)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_token	*find_next_operator_token(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return (NULL);
	current = current->next;
	while (current)
	{
		if (current->type > CMD)
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
	new_node->next = NULL;
	new_node->node_type = node->type;
	if (node->redirections)
		new_node->redirections = node->redirections;
	new_node->is_in_par = node->is_in_par;
	return (new_node);
}
