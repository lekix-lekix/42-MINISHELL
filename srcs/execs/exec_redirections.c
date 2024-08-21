/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 09:40:20 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/21 18:22:15 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_errmsg(void)
{
	printf("There was an error\n");
	return (1);
}

int	ft_append(t_redir *redirections, int *le_status)
{
	int	fd;

	if (!redirections->filename)
	{
		*le_status = ft_errmsg();
		return (*le_status);
	}
	fd = open(redirections->filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
	{
		printf("There was a problem opening the file\n");
		*le_status = ft_errmsg();
		return (*le_status);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	close(fd);
	*le_status = 0;
	return (0);
}

int	ft_in(t_redir *redirections, int *le_status)
{
	int	fd;

	if (!redirections->filename)
	{
		// printf("There wasn't a filename\n");
		*le_status = ft_errmsg();
		return (*le_status);
	}
	fd = open(redirections->filename, O_RDONLY);
	if (fd == -1)
	{
		// printf("There was a problem opening the file\n");
		*le_status = ft_errmsg();
		return (*le_status);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL));
	close(fd);
	*le_status = 0;
	return (*le_status);
}

int	ft_out(t_redir *redirections, int *status)
{
	int	fd;

	if (!redirections->filename)
	{
		*status = ft_errmsg();
		return (*status);
	}
	fd = open(redirections->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		*status = ft_errmsg();
		return (*status);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL));
	close(fd);
	*status = 0;
	return (*status);
}

int	ft_check_redirections(t_token *node)
{
	t_redir	*redirections;
	int		le_status;

	redirections = node->redirections;
	while (redirections)
	{
        // printf("HELLO IM NODE %s\n", node->contents[0]);
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
			(dup2(redirections->heredoc, 0), close(redirections->heredoc));
		redirections = redirections->next;
	}
	return (ENO_SUCCESS);
}
