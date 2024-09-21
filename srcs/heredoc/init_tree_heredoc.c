/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:56:13 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/21 13:05:56 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_reopen_stdin(char *line)
{
	free(line);
	line = NULL;
	open("/dev/stdout", O_RDONLY);
	return (1);
}

int	ft_write_heredoc_fd(t_redir *redirection, int tmp_file_fd)
{
	char	*tmp;
	char	*line;

	while (1)
	{
		signal(SIGINT, ft_sigint_handler_heredoc);
		signal(SIGQUIT, SIG_IGN);
		line = readline("> ");
		if (ft_shell()->heredoc_sigint)
			return (ft_reopen_stdin(line));
		if (!line)
			break ;
		if (ft_is_delimiter(redirection->filename, line))
			break ;
		else
		{
			tmp = ft_pre_expand(line);
			line = ft_join(tmp, "\n");
			if (write(tmp_file_fd, line, ft_strlen(line)) == -1)
				return (ft_exit_close(255), -1);
		}
	}
	ft_init_signals();
	return (0);
}

int	ft_heredoc(t_redir *redirection, char *tmp_file_path)
{
	int	tmp_file_fd;

	tmp_file_fd = open(tmp_file_path, O_RDWR | O_CREAT, 0644);
	if (tmp_file_fd == -1)
	{
		perror("bash: open");
		ft_exit_close(255);
	}
	if (ft_write_heredoc_fd(redirection, tmp_file_fd))
	{
		close(tmp_file_fd);
		return (1);
	}
	close(tmp_file_fd);
	redirection->filename = tmp_file_path;
	return (0);
}

static int	ft_init_leaf(t_ast *node)
{
	t_redir	*redirections;
	char	*tmp_file_path;

	redirections = node->token_node->redirections;
	while (redirections)
	{
		if (redirections->redir_type == REDIR_HEREDOC)
		{
			tmp_file_path = create_random_filename();
			if (ft_heredoc(redirections, tmp_file_path))
				return (-1);
		}
		redirections = redirections->next;
	}
	return (0);
}

int	ft_init_tree(t_ast *node)
{
	if (node->left)
		if (ft_init_tree(node->left) == -1)
			return (-1);
	if (node->node_type == CMD)
	{
		if (ft_init_leaf(node))
			return (-1);
		return (0);
	}
	if (node->right)
		if (ft_init_tree(node->right) == -1)
			return (-1);
	return (0);
}
