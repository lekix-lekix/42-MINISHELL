/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:04:20 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/20 13:43:22 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clear_content_lst(t_lst **lst)
{
	t_lst	*current;

	current = *lst;
	if (!current)
		return ;
	while (current)
	{
		free(current->content);
		current = current->next;
	}
}

void	flush_lst(int which_list, t_gbg *all_lsts)
{
	if (which_list == ENV)
		ft_lstclear(&all_lsts->mlc_env, free);
	if (which_list == PARSING)
		ft_lstclear(&all_lsts->mlc_parsing, free);
	if (which_list == ALL)
	{
		ft_lstclear(&all_lsts->mlc_env, free);
		ft_lstclear(&all_lsts->mlc_parsing, free);
	}
}

void	remove_free_mem_node(int which_lst, t_gbg *all_lsts, void *mem_addr)
{
	if (which_lst == ENV)
		remove_mem_node(&all_lsts->mlc_env, mem_addr);
	if (which_lst == PARSING)
		remove_mem_node(&all_lsts->mlc_parsing, mem_addr);
}

void	add_node_gbg(int which_lst, t_gbg *all_lsts, t_lst *node)
{
	if (which_lst == ENV)
		ft_lstadd_back(&all_lsts->mlc_env, node);
	else if (which_lst == PARSING)
		ft_lstadd_back(&all_lsts->mlc_parsing, node);
}

int	gbg_coll(void *mem_addr, int which_list, int rule)
{
	static t_gbg	all_gbg_lsts;
	t_lst			*node;

	if (rule == ADD)
	{
		node = ft_lstnew(mem_addr);
		if (!node)
			return (free(mem_addr), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
		add_node_gbg(which_list, &all_gbg_lsts, node);
	}
	if (rule == FREE)
		remove_free_mem_node(which_list, &all_gbg_lsts, mem_addr);
	if (rule == FLUSH_ONE)
		flush_lst(which_list, &all_gbg_lsts);
	if (rule == FLUSH_ALL)
		flush_lst(ALL, &all_gbg_lsts);
	return (0);
}
