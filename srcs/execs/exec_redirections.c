/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 09:40:20 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/25 15:30:09 by sabakar-         ###   ########.fr       */
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
		printf("The wasn't a filename\n");
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
	dup2(fd, STDOUT_FILENO);
	close(fd);
	*le_status = 0;
	return (0);
}

int	ft_in(t_redir *redirections, int *le_status)
{
	int	fd;

	if (!redirections->filename)
	{
		printf("There wasn't a filename\n");
		*le_status = ft_errmsg();
		return (*le_status);
	}
	fd = open(redirections->filename, O_RDONLY);
	if (fd == -1)
	{
		printf("There was a problem opening the file\n");
		*le_status = ft_errmsg();
		return (*le_status);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	*le_status = 0;
	return (*le_status);
}

int	ft_out(t_redir *redirections, int *status)
{
	int	fd;
	// int	original_stdout;
	// int	original_stdin;

	// original_stdout = dup(STDOUT_FILENO);
	// original_stdin = dup(STDIN_FILENO);
	// Do the redirections and execute the command...
	// Restore the original file descriptors
	// printf("The filename: %s\n", redirections->filename);
	if (!redirections->filename)
	{
		// printf("There wasn't a filename\n");
		*status = ft_errmsg();
		return (*status);
	}
	fd = open(redirections->filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		// printf("There was a problem opening the file\n");
		*status = ft_errmsg();
		return (*status);
	}
	printf("We are in out\n");
	dup2(fd, STDOUT_FILENO);
	close(fd);
	printf("We're here 56!\n");
	// dup2(original_stdout, STDOUT_FILENO);
	// dup2(original_stdin, STDIN_FILENO);
	// close(original_stdout);
	// close(original_stdin);
	*status = 0;
	return (*status);
}

int	ft_check_redirections(t_redir *redirections, t_minishell *data)
{
	int	le_status;

	while (redirections)
	{
		if (redirections->redir_type == REDIR_INPUT && ft_in(redirections,
				&le_status) != ENO_SUCCESS)
			return (le_status);
		else if (redirections->redir_type == REDIR_OUTPUT
			&& ft_out(redirections, &le_status) != ENO_SUCCESS)
		{
			printf("It's an output\n");
			return (le_status);
		}
		else if (redirections->redir_type == REDIR_OUTPUT_APPEND
			&& ft_append(redirections, &le_status) != ENO_SUCCESS)
			return (le_status);
		else if (redirections->redir_type == REDIR_HEREDOC
			&& ft_in(redirections, &le_status) != ENO_SUCCESS)
			(dup2(data->heredoc, 0), close(data->heredoc));
		redirections = redirections->next;
	}
	// printf("Finishing the loop\n");
	return (ENO_SUCCESS);
}
