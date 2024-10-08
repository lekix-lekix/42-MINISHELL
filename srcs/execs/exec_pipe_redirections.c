/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipe_redirections.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 16:01:26 by lekix             #+#    #+#             */
/*   Updated: 2024/09/11 20:30:49 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_pipe_redirections(t_redir *redirection, t_token *node)
{
	if (redirection->redir_type == REDIR_INPUT)
	{
		if (dup2(node->pipe_redir[0], STDIN_FILENO) == -1)
			return (perror("bash: dup2"), ft_exit_close(255), -1);
		ft_lst_del_pipe(&ft_shell()->pipes, node->pipe_redir[0]);
		close(node->pipe_redir[0]);
	}
	else if (redirection->redir_type == REDIR_OUTPUT)
	{
		if (dup2(node->pipe_redir[1], STDOUT_FILENO) == -1)
			return (perror("bash: dup2"), ft_exit_close(255), -1);
		ft_lst_del_pipe(&ft_shell()->pipes, node->pipe_redir[1]);
		close(node->pipe_redir[1]);
	}
	return (0);
}

int	*init_pipe(void)
{
	int	*pipe_fds;

	pipe_fds = malloc(sizeof(int) * 2);
	if (!pipe_fds || gbg_coll(pipe_fds, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	if (pipe(pipe_fds) == -1)
		return (perror("bash: pipe"), ft_exit_close(255), NULL);
	return (pipe_fds);
}
