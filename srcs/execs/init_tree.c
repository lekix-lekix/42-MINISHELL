/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_tree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/29 16:56:13 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 14:10:42 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// static void	ft_heredoc_sigint_handler(int signum)
// {
// 	(void)signum;
// 	// dprintf(2, "WE are here 15\n");
// 	close(ft_shell()->ft_stdin);
// 	close(ft_shell()->ft_stdout);
// 	gbg_coll(NULL, ALL, FLUSH_ALL);
// 	exit(SIGINT);
// }

void	ft_heredoc(t_redir *redirection, char *tmp_file_path)
{
	char	*line;
	char	*input;
	int		tmp_file_fd;

	// signal(SIGINT, ft_heredoc_sigint_handler);
	tmp_file_fd = open(tmp_file_path, O_RDWR | O_CREAT, 0644);
	if (tmp_file_fd == -1)
	{
		perror("bash: open");
		ft_exit_close(255);
	}
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (ft_is_delimiter(redirection->filename, line))
			break ;
		else
        {
            input = ft_pre_expand(line);
            line = ft_join(input, "\n");
            write(tmp_file_fd, line, ft_strlen(line));
			// ft_heredoc_expander(line, tmp_file_fd);
        }
		// free(line);
	}
	close(tmp_file_fd);
	redirection->filename = tmp_file_path;
}

// void	ft_heredoc(t_token *io, int p[2])
// {
// 	char	*line;
// 	char	*quotes;
// 	char	*input;

// 	signal(SIGINT, ft_heredoc_sigint_handler);
// 	quotes = io->content;
// 	// printf("WE are here 17\n");
// 	while (*quotes && *quotes != '"' && *quotes != '\'')
// 		quotes++;
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 			break ;
// 		if (ft_is_delimiter(io->redirections->filename, line))
// 			break ;
// 		if (!*quotes)
// 			ft_heredoc_expander(line, p[1]);
// 		else
// 		{
// 			input = ft_join(line, "\n");
// 			write(2, input, ft_strlen(line));
// 		}
// 		free(line);
// 	}
// 	close(p[0]);
// 	close(p[1]);
// 	gbg_coll(NULL, ALL, FLUSH_ALL);
// 	exit(0);
// }

// static bool	ft_leave_leaf(int p[2], int *pid)
// {
// 	waitpid(*pid, pid, 0);
// 	signal(SIGQUIT, ft_sigquit_handler);
// 	(ft_shell())->signint_child = false;
// 	close(p[1]);
// 	// close(p[0]);
// 	// close(ft_shell()->stdin);
// 	// close(ft_shell()->stdout);
// 	// printf("WE ARE HERE 28\n");
// 	if (WIFEXITED(*pid) && WEXITSTATUS(*pid) == SIGINT)
// 		return (true);
// 	return (false);
// }

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

void	read_into_filename(int fd, char *buf, char *filename)
{
	int	i;

	i = 0;
	ft_strlcpy(filename, "/tmp/", 6);
	i = -1;
	while (i < 4)
	{
		if (read(fd, buf + i, 1) == -1)
			ft_exit_close(255);
		if (ft_isalnum(buf[i]))
			i++;
	}
	buf[i] = '\0';
	ft_strlcat(filename, buf, 15);
}

char	*get_random_filename(void)
{
	char	*filename;
	char	buf[5];
	int		fd;

	filename = malloc(sizeof(char) * 16);
	if (!filename || gbg_coll(filename, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (perror("open"), ft_exit_close(255), NULL);
	ft_strlcpy(filename, "/tmp/", 6);
	read_into_filename(fd, buf, filename);
	close(fd);
	return (filename);
}

char	*create_random_filename(void)
{
	char	*filename;

	filename = get_random_filename();
	if (access(filename, F_OK) == -1)
		return (filename);
	else
		return (create_random_filename());
}

static void	ft_init_leaf(t_ast *node)
{
	t_token	*io;
	t_redir	*redirections;
	int		idx;
	char	**temp_contents;
	char	**la_args;
	char	*tmp_file_path;

	// int		p[2];
	// int		pid;
	idx = -1;
	io = node->token_node;
	temp_contents = NULL;
	redirections = node->token_node->redirections;
	while (io->contents[++idx])
	{
		la_args = ft_expand(io->contents[idx]);
		temp_contents = ft_concat_str_arr(temp_contents, la_args);
		// dprintf(2, "temp_contents = %s\n", temp_contents[0]);
	}
	io->contents = temp_contents;
	// printf("io->contents: %s\n", io->contents[1]);
	while (redirections)
	{
		if (redirections->redir_type == REDIR_HEREDOC)
		{
			tmp_file_path = create_random_filename();
			ft_heredoc(redirections, tmp_file_path);
			// pipe(p);
			// (signal(SIGQUIT, SIG_IGN));
			// (ft_shell())->signint_child = true;
			// pid = fork();
			// if (pid == 0)
			// 	ft_heredoc(io, p);
			// if (ft_leave_leaf(p, &pid))
			// 	return ;
			// redirections->heredoc = p[0];
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
