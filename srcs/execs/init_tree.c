/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:56:13 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/05 07:07:05 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_heredoc_sigint_handler(int signum)
{
	(void)signum;
	exit(SIGINT);
}

void	ft_heredoc(t_token *io, int p[2])
{
	char	*line;
	char	*quotes;

	signal(SIGINT, ft_heredoc_sigint_handler);
	quotes = io->content;
	while (*quotes && *quotes != '"' && *quotes != '\'')
		quotes++;
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
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

void	ft_sigquit_handler(int num)
{
	(void)num;
	ft_putstr_fd("Quit: 3\n", 1);
}

static bool	ft_leave_leaf(int p[2], int *pid)
{
	waitpid(*pid, pid, 0);
	signal(SIGQUIT, ft_sigquit_handler);
	(ft_shell())->signint_child = false;
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
	int	idx;
	int expanded_len;
	char *first_args;
	int	contents_len;

	idx = -1;
	io = node->token_node;
	// if (io->contents[1])
	// {	
	// 	printf("THE FIRST CONTENT: %s\n", io->contents[1]);
	// 	node->expanded_args = ft_expand(io->contents[1]);
	// }
	// contents_len = get_arr_len(io->contents);
	contents_len = 0;
	// printf("TLSD CONTENTS LEN: %d\n", contents_len);
	while (io->contents[++idx])
	{	
		(ft_shell())->expanded_args = ft_expand(io->contents[idx]);
		// printf("THE CONTENTS CONTENT: [%s]", io->contents[idx]);
		// int x = -1;
		// while ((ft_shell())->expanded_args[++x])
		// {	
		// 	printf("THe EX [%s]", (ft_shell())->expanded_args[x]);
		// 	io->contents[idx] = (ft_shell())->expanded_args[x];
		// }
		// ft_expand(io->contents[idx]);
		// printf("\n");
		contents_len++;
	}
	expanded_len = get_arr_len((ft_shell())->expanded_args);
	first_args = io->contents[0];
	io->contents = (char **)malloc(sizeof(char *) * (expanded_len + 2));
	// printf("THE FIRST OF CONTETNS: %s\n", first_args);
	// printf("THE LEN OF EXPANDED: %d\n", expanded_len);
	io->contents[0] = first_args;
	int g = -1;
	int c = 1;
	while (ft_shell()->expanded_args[++g] && contents_len > 1)
	{	
		// printf("hi!\n");
		// printf("THE EXPANDED ARGS: [%s]\n", (ft_shell())->expanded_args[g]);
		io->contents[c] = ft_shell()->expanded_args[g];
		c++;
	}
	io->contents[c] = 0;
	// int l = -1;
	// while (io->contents[++l])
		// printf("THE CONTENTS: %s\n", io->contents[1]);
	// printf("\n");
	while (io)
	{
		if (io->redirections && io->redirections->redir_type == REDIR_HEREDOC)
		{
			pipe(p);
			(ft_shell())->signint_child = true;
			signal(SIGQUIT, SIG_IGN),
			pid = (fork());
			if (!pid)
				ft_heredoc(io, p);
			if (ft_leave_leaf(p, &pid))
				return ;
			(ft_shell())->heredoc = p[0];
		}
		// else
			// printf("HERE YOU ARE\n");
		io = io->next;
	}
}

// void	ft_init_tree(t_ast *node)
// {
// 	if (!node)
// 		return ;
// 	if (node->node_type == PIPE || node->node_type == AND
// 		|| node->node_type == OR)
// 	{
// 		ft_init_tree(node->left);
// 		if (!(ft_shell())->heredoc_sigint)
// 			ft_init_tree(node->right);
// 	}
// 	else
// 		ft_init_leaf(node);
// }

void ft_init_tree(t_ast *node)
{
	if (node->left)
		ft_init_tree(node->left);
	if (node->node_type == CMD)
		ft_init_leaf(node);
	if (node->right)
		ft_init_tree(node->right);
}
