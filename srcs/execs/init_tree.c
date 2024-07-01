/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:56:13 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/01 17:38:01 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// static void	ft_heredoc_sigint_handler(int signum)
// {
// 	(void)signum;
// 	exit(SIGINT);
// }

void	ft_heredoc(t_token *io, int p[2])
{
	char	*line;
	char	*quotes;

	// signal(SIGINT, ft_heredoc_sigint_handler);
	quotes = io->content;
	print_lst(&io);
	printf("THe quotes: %s\n", quotes);
	while (*quotes && *quotes != '"' && *quotes != '\'')
		quotes++;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		// printf("The line is %s\n", line);
		// printf("The EOF is: %s\n", io->redirections->filename);
		if (ft_is_delimiter(io->redirections->filename, line))
			break ;
		if (!*quotes)
			ft_heredoc_expander(line, p[1]);
		else
		{
			ft_putstr_fd(line, p[1]);
			ft_putstr_fd("\n", p[1]);
		}
	}
	exit(0);
}

// void	ft_sigquit_handler(int num)
// {
// 	(void)num;
// 	ft_putstr_fd("Quit: 3\n", 1);
// }

static bool	ft_leave_leaf(int p[2], int *pid)
{
	waitpid(*pid, pid, 0);
	// signal(SIGQUIT, ft_sigquit_handler);
	// (ft_shell())->signint_child = false;
	close(p[1]);
	if (WIFEXITED(*pid) && WEXITSTATUS(*pid) == SIGINT)
		return (true);
	return (false);
}

static void	ft_init_leaf(t_ast *node)
{
	t_token	*io;
	int		p[2];
	int		pid;

	io = node->token_node;
	if (io->type == REDIR_HEREDOC)
		printf("The type is: %u\n", (io->type));
	while (io)
	{
		if (io->redirections && io->redirections->redir_type == REDIR_HEREDOC)
		{
			pipe(p);
			// (ft_shell())->signint_child = true;
			// signal(SIGQUIT, SIG_IGN)
			pid = (fork());
			if (!pid)
				ft_heredoc(io, p);
			if (ft_leave_leaf(p, &pid))
				return ;
			printf("1000!\n");
			(ft_shell())->heredoc = p[0];
		}
		// else
			// printf("Here u go!\n");
		io = io->next;
	}
}

void	ft_init_tree(t_ast *node)
{
	if (!node)
		return ;
	// printf("it's here mf\n");
	if (node->node_type == PIPE || node->node_type == AND
		|| node->node_type == OR)
	{
		ft_init_tree(node->left);
		if (!(ft_shell())->heredoc_sigint)
			ft_init_tree(node->right);
	}
	else
		ft_init_leaf(node);
}