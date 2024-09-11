/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 09:40:20 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 20:31:14 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_append(t_redir *redirections, int *le_status)
{
	int		fd;
	char	*err;

	fd = open(redirections->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		err = ft_join("bash: ", redirections->filename);
		perror(err);
		*le_status = 1;
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_exit_close(255), -1);
	close(fd);
	*le_status = 0;
	return (0);
}

int	ft_in(t_redir *redirections, int *le_status)
{
	int		fd;
	char	*err;

	fd = open(redirections->filename, O_RDONLY);
	if (fd == -1)
	{
		err = ft_join("bash: ", redirections->filename);
		perror(err);
		*le_status = 1;
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		return (ft_exit_close(255), -1);
	close(fd);
	*le_status = 0;
	return (*le_status);
}

int	ft_out(t_redir *redirections, int *status)
{
	int		fd;
	char	*err;

	fd = open(redirections->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		err = ft_join("bash: ", redirections->filename);
		perror(err);
		*status = 1;
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (ft_exit_close(255), -1);
	close(fd);
	*status = 0;
	return (*status);
}

int	ft_redir_heredoc(t_redir *redirection, int *le_status)
{
	int	heredoc_fd;

	heredoc_fd = open(redirection->filename, O_RDONLY);
	if (heredoc_fd == -1)
	{
		perror("bash: open");
		ft_exit_close(255);
	}
	if (dup2(heredoc_fd, STDIN_FILENO) == -1)
		return (ft_exit_close(255), -1);
	close(heredoc_fd);
	unlink(redirection->filename);
	*le_status = 0;
	return (*le_status);
}

int	ft_check_redirections(t_token *node)
{
	t_redir	*redirections;
	int		le_status;

	redirections = node->redirections;
	expand_redirections(&redirections);
	while (redirections)
	{
		if (ft_strcmp(redirections->filename, "pipe") == 0)
			handle_pipe_redirections(redirections, node);
		else if (redirections->redir_type == REDIR_INPUT && ft_in(redirections,
				&le_status) != ENO_SUCCESS)
			return (le_status);
		else if (redirections->redir_type == REDIR_OUTPUT
			&& ft_out(redirections, &le_status) != ENO_SUCCESS)
			return (le_status);
		else if (redirections->redir_type == REDIR_OUTPUT_APPEND
			&& ft_append(redirections, &le_status) != ENO_SUCCESS)
			return (le_status);
		else if (redirections->redir_type == REDIR_HEREDOC)
			return (ft_redir_heredoc(redirections, &le_status));
		redirections = redirections->next;
	}
	return (ENO_SUCCESS);
}
