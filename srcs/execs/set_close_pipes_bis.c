/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_close_pipes_bis.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:23:03 by lekix             #+#    #+#             */
/*   Updated: 2024/09/11 18:29:59 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_lst	*create_lst_node(void *mem_addr)
{
	t_lst	*lst_node;

	lst_node = malloc(sizeof(t_lst));
	if (!lst_node || gbg_coll(lst_node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	lst_node->content = mem_addr;
	lst_node->next = NULL;
	return (lst_node);
}

int	close_pipes_lst(t_lst **pipe_lst)
{
	t_lst	*current;
	int		*pipe;

	current = *pipe_lst;
	if (!current)
		return (-1);
	while (current)
	{
		pipe = (int *)current->content;
		if (*pipe != -1)
		{
			close(pipe[0]);
			pipe[0] = -1;
		}
		current = current->next;
	}
	close(ft_shell()->ft_stdin);
	close(ft_shell()->ft_stdout);
	return (0);
}

void	ft_lst_del_pipe(t_lst **lst, int pipe)
{
	t_lst	*current;
	t_lst	*prev;

	current = *lst;
	if (!current)
		return ;
	if (*(int *)current->content == pipe)
	{
		*lst = current->next;
		return ;
	}
	while (current)
	{
		if (*(int *)current->content == pipe)
		{
			prev->next = current->next;
			return ;
		}
		prev = current;
		current = current->next;
	}
}
