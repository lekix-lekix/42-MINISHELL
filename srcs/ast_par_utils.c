/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_par_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 17:11:53 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/28 17:12:45 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*find_last_par(t_token **lst)
{
	t_token	*current;
	t_token	*par;

	current = *lst;
	par = NULL;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->type == PAR_RIGHT)
			par = current;
		current = current->next;
	}
	return (par);
}

t_token	*find_closing_par(t_token **lst)
{
	t_token	*current;

	current = (*lst)->next;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->type == PAR_LEFT)
			return (find_last_par(lst));
		if (current->type == PAR_RIGHT)
		{
			if (current->next)
				return (current);
		}
		current = current->next;
	}
	return (NULL);
}

t_token	*find_right_par(t_token **lst)
{
	t_token	*current;

	current = *lst;
	while (current)
	{
		if (current->type == PAR_RIGHT)
			return (current);
		current = current->next;
	}
	return (current);
}
