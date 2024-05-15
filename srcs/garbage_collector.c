/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 16:04:20 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/15 14:47:30 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	remove_mem_node(t_lst **lst, void *mem_addr)
{
	t_lst	*to_remove;
	int		nullify;

	nullify = 0;
	if (!*lst)
		return ;
	if (ft_lstsize(*lst) == 1)
		nullify = 1;
	to_remove = ft_lstfind_node(lst, mem_addr);
	if (to_remove)
	{
		free(to_remove->content);
		free(to_remove);
		if (nullify)
			*lst = NULL;
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
