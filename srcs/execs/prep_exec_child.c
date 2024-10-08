/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:34:26 by lekix             #+#    #+#             */
/*   Updated: 2024/09/19 11:50:34 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_child(t_ast *node)
{
	pid_t	*pid;
	t_lst	*pid_node;

	pid = malloc(sizeof(pid_t));
	if (!pid || gbg_coll(pid, PARSING, ADD))
		return (ft_exit_close(255), -1);
	*pid = fork();
	if (*pid == -1)
		return (perror("bash: fork: "), ft_exit_close(255), -1);
	if (*pid == 0)
	{
		ft_shell()->signint_child = true;
		ft_check_cmds(node->token_node);
	}
	pid_node = create_lst_node(pid);
	ft_lstadd_back(&ft_shell()->pids, pid_node);
	return (0);
}

int	*exec_child_next_not_par(t_ast *to_exec, int *pipe_fds)
{
	set_pipe_redir_in(to_exec->next, pipe_fds[0]);
	set_pipe_redir_out(to_exec, pipe_fds[1]);
	exec_child(to_exec);
	close(pipe_fds[1]);
	close_pipe_redir_in(to_exec);
	return (NULL);
}

int	*exec_child_next_in_par(t_ast *to_exec, int *pipe_fds)
{
	set_pipe_redir_out(to_exec, pipe_fds[1]);
	exec_child(to_exec);
	close_pipe_redir_in(to_exec);
	close(pipe_fds[1]);
	return (pipe_fds);
}

int	*exec_child_no_next(t_ast *to_exec)
{
	exec_child(to_exec);
	close_pipe_redir_in(to_exec);
	return (NULL);
}

int	*prep_exec_child(t_ast *to_exec)
{
	int		*pipe_fds;
	t_lst	*pipe_node;

	pipe_fds = NULL;
	if (to_exec->next)
	{
		pipe_fds = init_pipe();
		pipe_node = create_lst_node(&pipe_fds[0]);
		ft_lstadd_back(&ft_shell()->pipes, pipe_node);
		pipe_node = create_lst_node(&pipe_fds[1]);
		ft_lstadd_back(&ft_shell()->pipes, pipe_node);
		return (exec_child_next_not_par(to_exec, pipe_fds));
	}
	else
		return (exec_child_no_next(to_exec));
}
