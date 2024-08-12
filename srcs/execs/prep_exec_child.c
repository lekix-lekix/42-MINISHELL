/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prep_exec_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 15:34:26 by lekix             #+#    #+#             */
/*   Updated: 2024/08/12 05:51:40 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	exec_child(t_ast *node)
{
	pid_t	*pid;
	t_lst	*pid_node;
	
	pid = malloc(sizeof(pid_t));
	if (!pid || gbg_coll(pid, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	*pid = fork();
	if (*pid == -1)
		return (perror("bash: fork: "), gbg_coll(NULL, ALL, FLUSH_ALL),
			exit(255), -1);
	if (*pid == 0)
		ft_check_cmds(node->token_node);
	pid_node = create_lst_node(pid);
	ft_lstadd_back(&ft_shell()->pids, pid_node);
	ft_shell()->pids_num += 1;
	return (0);
}

int	*exec_child_next_not_par(t_ast *to_exec, int *pipe_fds)
{
	set_pipe_redir_in(to_exec->next, pipe_fds[0]);
	set_pipe_redir_out(to_exec, pipe_fds[1]);
	exec_child(to_exec);
	close_pipe_redir_in(to_exec);
	close(pipe_fds[1]);
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
	}
	if (to_exec->next && !to_exec->next->is_in_par)
		return (exec_child_next_not_par(to_exec, pipe_fds));
	else if (to_exec->next && to_exec->next->is_in_par)
		return (exec_child_next_in_par(to_exec, pipe_fds));
	else
		return (exec_child_no_next(to_exec));
}
