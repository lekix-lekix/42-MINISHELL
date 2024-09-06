/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 00:17:24 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/06 15:36:13 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_is_delimiter(char *delimiter, char *str)
{
	while (*str)
	{
		if (*delimiter == '"' || *delimiter == '\'')
		{
			delimiter++;
			continue ;
		}
		else if (*str == *delimiter)
		{
			str++;
			delimiter++;
		}
		else
			return (false);
	}
	while (*delimiter == '"' || *delimiter == '\'')
		delimiter++;
	return (!*delimiter);
}

void	ft_reset_ports(bool piped)
{
	if (piped)
		return ;
	dup2((ft_shell())->ft_stdin, 0);
	dup2((ft_shell())->ft_stdout, 1);
}

int	ft_get_la_status(int la_status)
{
	if (WIFSIGNALED(la_status))
		return (128 + WTERMSIG(la_status));
	return (WEXITSTATUS(la_status));
}

int	wait_all_pids(t_lst **pids)
{
	t_lst	*current;
	int		status;

	current = *pids;
	status = -1;
	while (current)
	{
		waitpid(*(pid_t *)current->content, &status, WUNTRACED);
		if (WIFEXITED(status))
			ft_shell()->exit_status = WEXITSTATUS(status);
		current = current->next;
	}
	return (0);
}

int	close_wait(int par_pid)
{
	close_pipes_lst(&ft_shell()->pipes);
	if (par_pid != -1)
		waitpid(par_pid, NULL, WUNTRACED);
	wait_all_pids(&ft_shell()->pids);
	ft_shell()->pids = NULL;
	return (0);
}

static void	ft_del(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	*ft_unset_cleaner(void *ptr, bool clean)
{
	static t_lst	*clean_list;

	if (clean)
	{
		ft_lstclear(&clean_list, ft_del);
		return (NULL);
	}
	else
	{
		ft_lstadd_back(&clean_list, ft_lstnew(ptr));
		return (ptr);
	}
}
