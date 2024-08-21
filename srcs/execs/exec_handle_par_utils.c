/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:54:20 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/20 16:02:12 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*find_original_token_lst(t_token **lst, t_token *to_find)
{
	t_token	*current;

	current = *lst;
	while (current)
	{
		if (current->original_token == to_find->original_token)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	delete_begin_end_par_nodes(t_token **lst)
{
	t_token	*current;
	t_token	*end_par_node;

	current = *lst;
	if (current->type == PAR_LEFT)
	{
		end_par_node = find_closing_par(lst);
		*lst = current->next;
		current->next = NULL;
	}
	current = *lst;
	while (current && current->next)
	{
		if (current->next == end_par_node)
		{
			current->next = NULL;
			return ;
		}
		current = current->next;
	}
}

void	set_par_node(t_token **lst, t_token *node)
{
	t_token	*current;
	int		par;

	current = *lst;
	par = 0;
	while (current)
	{
		if (current->type == PAR_LEFT)
			par++;
		if (current->type == PAR_RIGHT)
			par--;
		if (current == node)
		{
			if (!par)
				current->is_in_par = 0;
			else
				current->is_in_par = 1;
			return ;
		}
		current = current->next;
	}
}

void	set_par_lst(t_token **lst)
{
	t_token	*current;

	current = *lst;
	while (current)
	{
		set_par_node(lst, current);
		current = current->next;
	}
}

t_token	*find_left_par_original_token(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*par;

	current = *lst;
	par = NULL;
	while (current)
	{
		if (current->type == PAR_LEFT)
			par = current;
		if (current == node)
			return (par);
		current = current->next;
	}
	return (par);
}
