/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 10:25:42 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/19 11:45:40 by sabakar-         ###   ########.fr       */
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
	// (ft_shell())->signint_child = true;
	// (ft_shell())->heredoc_sigint = true;
	exit(SIGINT);
}

// void	ft_heredoc(t_token *io)
// {
// 	char	*line;
// 	char	*quotes;

// 	signal(SIGINT, ft_heredoc_sigint_handler);
// 	quotes = io->content;
// 	printf("WE are here 17\n");
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
// 			ft_putstr_fd(line, p[1]);
// 			ft_putstr_fd("\n", p[1]);
// 		}
// 		free(line);
// 	}
// 	// close(p[0]);
// 	// close(p[1]);
// 	exit(0);
// }

static bool	ft_leave_leaf(int *pid)
{
	waitpid(*pid, pid, 0);
	signal(SIGQUIT, ft_sigquit_handler);
	(ft_shell())->signint_child = false;
	// close(p[1]);
	// close(p[0]);
	// close(ft_shell()->ft_stdin);
	// close(ft_shell()->ft_stdout);
	printf("WE ARE HERE 28\n");
	if (WIFEXITED(*pid) && WEXITSTATUS(*pid) == SIGINT)
		return (true);
	return (false);
}

int	ft_write_heredoc_fd(t_redir *redirection, int tmp_file_fd)
{
	// char	*tmp;
	char	*line;
	int		f;

	f = fork();
	signal(SIGINT, ft_heredoc_sigint_handler);
	// printf("%d\n", &ft_heredoc_sigint_handler);
	if (!f)
	{
		while (1)
		{
			// line = readline("> ");
            // // if (ft_leave_leaf(&f))
			// // 	exit (130) ;
			// if (!line)
			// 	break ;
			// if (ft_is_delimiter(redirection->filename, line))
			// 	break ;
			// else
			// {
			// 	tmp = ft_pre_expand(line);
			// 	line = ft_join(tmp, "\n");
			// 	if (write(tmp_file_fd, line, ft_strlen(line)) == -1)
			// 		return (ft_exit_close(255), -1);
			// }
            line = readline("> ");
            if (!line)
                break ;
            if (ft_is_delimiter(redirection->filename, line))
                break ;
            // if (!*quotes)
            //     ft_heredoc_expander(line, p[1]);
            else
            {
                ft_putstr_fd(line, tmp_file_fd);
                ft_putstr_fd("\n", tmp_file_fd);
            }
            free(line);
		}
        free(line);
	    exit(130);
	}
	// exit(130);
	return (0);
}

void	ft_heredoc(t_redir *redirection)
{
	int	tmp_file_fd;

	(ft_shell())->tmp_file_path = create_random_filename();
	tmp_file_fd = open((ft_shell())->tmp_file_path, O_RDWR | O_CREAT, 0644);
	if (tmp_file_fd == -1)
	{
		perror("bash: open");
		ft_exit_close(255);
	}
	ft_write_heredoc_fd(redirection, tmp_file_fd);
	close(tmp_file_fd);
	redirection->filename = (ft_shell())->tmp_file_path;
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
	t_redir	*redirections;
	int		pid;

	redirections = node->token_node->redirections;
	while (redirections)
	{
		if (redirections->redir_type == REDIR_HEREDOC)
		{
			(signal(SIGQUIT, SIG_IGN));
			(ft_shell())->signint_child = true;
			pid = fork();
			if (pid == 0)
			    ft_heredoc(redirections);
            if (ft_leave_leaf(&pid))
			    return ;
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