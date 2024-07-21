/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_redirections.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 09:40:20 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/21 01:46:51 by lekix            ###   ########.fr       */
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
	dup2(fd, STDOUT_FILENO);
	close(fd);
	*status = 0;
	return (*status);
}

void    ft_lst_del_pipe(t_lst **lst, int pipe)
{
    t_lst *current;
    t_lst *prev;

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

int	handle_pipe_redirections(t_redir *redirection, t_token *node)
{
	if (redirection->redir_type == REDIR_INPUT)
	{
        printf("node dup2 input = %s pipe fd = %d\n", node->contents[0], node->pipe_redir[0]);
		if (dup2(node->pipe_redir[0], STDIN_FILENO) == -1)
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
        ft_lst_del_pipe(&ft_shell()->pipes, node->pipe_redir[0]);
        close(node->pipe_redir[0]);
	}
	else if (redirection->redir_type == REDIR_OUTPUT)
	{
        printf("node dup2 output = %s\n", node->contents[0]);
		if (dup2(node->pipe_redir[1], STDOUT_FILENO) == -1)
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
        ft_lst_del_pipe(&ft_shell()->pipes, node->pipe_redir[1]);
        close(node->pipe_redir[1]);
	}
    
	return (0);
}

int	ft_check_redirections(t_token *node)
{
	t_redir	*redirections;
	int		le_status;

	redirections = node->redirections;
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
			(dup2((ft_shell())->heredoc, 0), close((ft_shell())->heredoc));
		redirections = redirections->next;
	}
	return (ENO_SUCCESS);
}
