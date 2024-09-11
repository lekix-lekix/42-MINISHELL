/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils_bis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:08:49 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 18:10:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
