/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 15:54:20 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/24 18:02:57 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*find_original_token_lst(t_token **lst, t_token *to_find)
{
	t_token	*current;

	current = *lst;
	printf("find original =====\n");
	if (to_find->type == CMD)
		printf("to find = %s %s original %s %s\n", to_find->contents[0],
			to_find->contents[1], to_find->original_token->contents[0],
			to_find->original_token->contents[1]);
	else
		printf("to find op %d, original %d\n", to_find->type,
			to_find->original_token->type);
	while (current)
	{
		if (current->type == CMD)
			printf("current = %s %s original %s %s\n", current->contents[0],
				current->contents[1], current->original_token->contents[0],
				current->original_token->contents[1]);
		else
			printf("current op %d, original %d\n", current->type,
				current->original_token->type);
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

	end_par_node = NULL;
	current = *lst;
	if (current->type == PAR_LEFT)
	{
		end_par_node = find_closing_par(lst);
		*lst = current->next;
		current->next = NULL;
	}
	current = *lst;
	if (!end_par_node)
		return ;
	while (current && current->next)
	{
		printf("end par node = %d\n", end_par_node->type);
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
