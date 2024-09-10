/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:56:13 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/09 21:18:50 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_heredoc_sigint_handler(int signum)
{
	(void)signum;
	printf("WE are here 15\n");
	close(ft_shell()->ft_stdin);
	close(ft_shell()->ft_stdout);
	gbg_coll(NULL, ALL, FLUSH_ALL);
	exit(SIGINT);
}

void	ft_heredoc(t_token *io, int p[2])
{
	char	*line;
	char	*quotes;
    char    *input;

	signal(SIGINT, ft_heredoc_sigint_handler);
	quotes = io->content;
	printf("WE are here 17\n");
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
            input = ft_join(line, "\n");
            write(2, input, ft_strlen(line));
		}
		free(line);
	}
    close(p[0]);
    close(p[1]);
    gbg_coll(NULL, ALL, FLUSH_ALL);
	exit(0);
}

static bool	ft_leave_leaf(int p[2], int *pid)
{
	waitpid(*pid, pid, 0);
	signal(SIGQUIT, ft_sigquit_handler);
	(ft_shell())->signint_child = false;
	close(p[1]);
	// close(p[0]);
	// close(ft_shell()->ft_stdin);
	// close(ft_shell()->ft_stdout);
	printf("WE ARE HERE 28\n");
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
	res = (char **)malloc(sizeof(char *) * (total_len + 1));
	if (!res || gbg_coll(res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	x = -1;
	while (++x < len1)
		res[x] = msh_strdup(arr[x], PARSING);
	y = -1;
	while (++y < len2)
	{
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
			{	
				ft_heredoc(io, p);
				// close(ft_shell()->ft_stdin);
				// close(ft_shell()->ft_stdout);
			}
			if (ft_leave_leaf(p, &pid))
				return ;
			redirections->heredoc = p[0];
		}
		redirections = redirections->next;
	}
	// close(p[1]);
	// close(p[0]);
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
