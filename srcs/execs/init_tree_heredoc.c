/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree_heredoc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:56:13 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/19 11:50:28 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// static void	ft_heredoc_sigint_handler(int signum)
// {
// 	(void)signum;
// 	printf("WE are here 15\n");
// 	close(ft_shell()->ft_stdin);
// 	close(ft_shell()->ft_stdout);
// 	gbg_coll(NULL, PARSING, FLUSH_ALL);
// 	// gbg_coll(NULL, ALL, FLUSH_ALL);
// 	// ft_close_fds();
// 	// (ft_shell())->is_ctrl_c = 23;
// 	exit(130);
// 	// return (1);
// }

// static volatile sig_atomic_t	g_signal_caught = 0;
// static int sig_atomic_t g_signal_caught = 0;
// signal(SIGINT, ft_heredoc_sigint_handler);

int	ft_write_heredoc_fd(t_redir *redirection, int tmp_file_fd)
{
	char	*tmp;
	char	*line;

	while (1)
	{
		line = readline("> ");
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
	return (0);
}

void	ft_heredoc(t_redir *redirection, char *tmp_file_path)
{
	int	tmp_file_fd;

	tmp_file_fd = open(tmp_file_path, O_RDWR | O_CREAT, 0644);
	if (tmp_file_fd == -1)
	{
		perror("bash: open");
		ft_exit_close(255);
	}
	ft_write_heredoc_fd(redirection, tmp_file_fd);
	close(tmp_file_fd);
	redirection->filename = tmp_file_path;
}

char	**ft_concat_str_arr(char **arr, char **arr2)
{
	int		len1;
	int		len2;
	char	**res;
	int		x;
	int		y;

	len1 = get_arr_len(arr);
	len2 = get_arr_len(arr2);
	res = (char **)malloc(sizeof(char *) * (len1 + len2 + 2));
	if (!res || gbg_coll(res, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	x = -1;
	while (++x < len1)
		res[x] = msh_strdup(arr[x], PARSING);
	y = -1;
	while (++y < len2)
	{
		res[x] = msh_strdup(arr2[y], PARSING);
		x++;
	}
	res[x] = NULL;
	return (res);
}

static void	ft_init_leaf(t_ast *node)
{
	t_redir	*redirections;
	char	*tmp_file_path;

	redirections = node->token_node->redirections;
	while (redirections)
	{
		if (redirections->redir_type == REDIR_HEREDOC)
		{
			tmp_file_path = create_random_filename();
			ft_heredoc(redirections, tmp_file_path);
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
