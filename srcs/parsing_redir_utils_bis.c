/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir_utils_bis.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 17:56:28 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 19:45:00 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_redir_lst(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return ;
	while (current)
	{
		if (is_a_redir_operator(current) && current->next
			&& current->next->type == CMD)
			current->next->type = OUTFILE;
		current = current->next;
	}
}

void	set_args_lst(t_token **lst)
{
	t_token	*current;
	int		found_cmd;

	current = *lst;
	if (!current)
		return ;
	found_cmd = 0;
	while (current)
	{
		if (current->type == CMD && found_cmd)
			current->type = ARGS_FLAGS;
		if (current->type == CMD)
		{
			found_cmd = 1;
			current = current->next;
			continue ;
		}
		if (is_a_token_operator(current))
			found_cmd = 0;
		current = current->next;
	}
}

void	add_redirection_node(t_redir **lst, t_redir *node)
{
	t_redir	*current;

	current = *lst;
	if (!current)
	{
		*lst = node;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = node;
}

void	add_front_redir_node(t_redir **lst, t_redir *node)
{
	t_redir	*current;

	current = *lst;
	if (!current)
	{
		*lst = node;
		return ;
	}
	node->next = current;
	*lst = node;
}

t_redir	*get_redir_lst_par(t_token **redir_node_lst)
{
	t_token	*current;
	t_redir	*new_redir_node;
	t_redir	*redir_lst;

	current = *redir_node_lst;
	redir_lst = NULL;
	while (current && (is_a_redir_operator(current)
			|| current->type == OUTFILE))
	{
		if (is_a_redir_operator(current) && current->next
			&& current->next->type == OUTFILE)
		{
			new_redir_node = create_redir_node(REDIR_OUTPUT_APPEND,
					current->next->content);
			remove_token_node(redir_node_lst, current->next);
			remove_token_node(redir_node_lst, current);
			add_redirection_node(&redir_lst, new_redir_node);
			current = *redir_node_lst;
			continue ;
		}
		current = current->next;
	}
	return (redir_lst);
}
