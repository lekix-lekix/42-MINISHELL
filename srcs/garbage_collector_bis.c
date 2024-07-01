/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector_bis.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 17:40:49 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/28 17:40:58 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_lst_mem_node(t_lst **lst, void *mem_addr)
{
	t_lst	*current;
	t_lst	*prev;

	current = *lst;
	while (current)
	{
		if (current->content == mem_addr)
		{
			prev->next = current->next;
			free(current->content);
			free(current);
			break ;
		}
		prev = current;
		current = current->next;
	}
}

void	remove_mem_node(t_lst **lst, void *mem_addr)
{
	t_lst	*current;
	int		nullify;

	nullify = 0;
	if (!*lst)
		return ;
	if (ft_lstsize(*lst) == 1)
		nullify = 1;
	current = *lst;
	if (current->content == mem_addr)
	{
		*lst = current->next;
		free(current->content);
		free(current);
		return ;
	}
	if (nullify)
		*lst = NULL;
}
