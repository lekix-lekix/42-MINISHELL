/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:56:13 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/01 18:01:24 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_heredoc_sigint_handler(int signum)
{
	(void)signum;
	gbg_coll(NULL, ENV, FREE);
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
		free(line);
	}
	exit(0);
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

char	**ft_concat_str_arr(char **arr, char **arr2)
{
	int		len1;
	int		len2;
	char	**res;
	int		x;
	int		y;
	int		total_len;

	len1 = get_arr_len(arr);
	len2 = get_arr_len(arr2);
	total_len = len1 + len2;
	// printf("THE TOTAL LEN IS: %d\n", total_len);
	res = (char **)malloc(sizeof(char *) * (total_len + 1));
	if (!res || gbg_coll(res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	x = -1;
	while (++x < len1)
	{
		// if (!arr[x] || !*arr[x])
		// 	continue ;
		res[x] = msh_strdup(arr[x], PARSING);
	}
	y = -1;
	while (++y < len2)
	{
		// if (!arr2[y] || !*arr2[y])
		// 	continue ;
		res[x] = msh_strdup(arr2[y], PARSING);
		x++;
	}
	res[total_len] = NULL;
	return (res);
}

static void	ft_init_leaf(t_ast *node)
{
	t_token	*io;
	t_redir	*redirections;
	int		p[2];
	int		pid;
	int		idx;
	char	**temp_contents;
	char	**la_args;

	idx = -1;
	io = node->token_node;
	temp_contents = NULL;
	redirections = node->token_node->redirections;
	while (io->contents[++idx])
	{
		la_args = ft_expand(io->contents[idx]);
		// printf("THE RETURNED SH!T: %s\n", la_args[0]);
		temp_contents = ft_concat_str_arr(temp_contents, la_args);
	}
	io->contents = temp_contents;
	while (redirections)
	{
		if (redirections->redir_type == REDIR_HEREDOC)
		{
			pipe(p);
			(signal(SIGQUIT, SIG_IGN));
			(ft_shell())->signint_child = true;
			pid = fork();
			if (pid == 0)
				ft_heredoc(io, p);
			if (ft_leave_leaf(p, &pid))
				return ;
			redirections->heredoc = p[0];
		}
		redirections = redirections->next;
	}
}

void	ft_init_tree(t_ast *node)
{
	if (node->left)
		ft_init_tree(node->left);
	if (node->node_type == CMD)
		ft_init_leaf(node);
	if (node->right)
		ft_init_tree(node->right);
}
