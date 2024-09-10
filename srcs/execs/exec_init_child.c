/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:17:42 by lekix             #+#    #+#             */
/*   Updated: 2024/09/10 22:38:55 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_cmds(t_token *token_node)
{
	int	la_status;

	la_status = ft_check_redirections(token_node);
	close_pipes_lst(&ft_shell()->pipes);
	if (la_status != ENO_SUCCESS)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(la_status), -1);
	if (ft_is_builtin(token_node->contents[0]))
	{
		la_status = ft_exec_builtins(token_node->contents);
		close(ft_shell()->ft_stdin);
		close(ft_shell()->ft_stdout);
		gbg_coll(NULL, ALL, FLUSH_ALL);
		exit(la_status);
	}
	else
		la_status = ft_exec_non_builtins(token_node);
	return (la_status);
}

int	init_only_child_no_fork(t_token *node)
{
	pid_t	pid;
	int		status;

    ft_update_envlst("_", node->contents[get_arr_len(node->contents) - 1], false);
	if (!node->contents || !node->contents[0] || !node->contents[0][0])
	{
		ft_shell()->exit_status = ft_check_redirections(node);
		return (ft_reset_ports(false), ft_close_fds(), 0);
	}
	ft_shell()->exit_status = ft_check_redirections(node);
	if (ft_shell()->exit_status != ENO_SUCCESS)
		return (ft_reset_ports(false), ft_close_fds(), -1);
	if (ft_is_builtin(node->contents[0]))
	{
		ft_shell()->exit_status = ft_exec_builtins(node->contents);
		return (ft_reset_ports(false), ft_close_fds(), 0);
	}
	pid = fork();
	if (pid == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), perror("bash: fork"), exit(255),
			-1);
	if (pid == 0)
	{
		(ft_shell())->signint_child = true;
		status = ft_exec_non_builtins(node);
		// dprintf(2, "WE arrives here 13\n");
	}
	(ft_reset_ports(false), ft_close_fds(), waitpid(pid, &status, WUNTRACED));
	if (WIFEXITED(status))
		ft_shell()->exit_status = WEXITSTATUS(status);
	return (0);
}
