/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:17:42 by lekix             #+#    #+#             */
/*   Updated: 2024/09/11 23:35:41 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_check_cmds(t_token *token_node)
{
	int	la_status;

	la_status = ft_check_redirections(token_node);
	close_pipes_lst(&ft_shell()->pipes);
	if (la_status != ENO_SUCCESS)
		return (ft_exit_close(255), -1);
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

int	exec_non_builtin_solo(t_token *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (perror("bash: fork"), ft_exit_close(255), -1);
	if (pid == 0)
	{
		(ft_shell())->signint_child = true;
		status = ft_exec_non_builtins(node);
	}
	(ft_reset_ports(false), ft_close_fds(), waitpid(pid, &status, WUNTRACED));
	if (WIFEXITED(status))
		ft_shell()->exit_status = WEXITSTATUS(status);
	return (0);
}

int	init_only_child_no_fork(t_token *node)
{
	ft_update_envlst("_", node->contents[get_arr_len(node->contents) - 1],
		false);
	if (!node->contents || !node->contents[0] || !node->contents[0][0])
	{
        if (!node->redirections)
        {
            write(2, "bash: : command not found\n", 27);
            return (ft_close_fds(), gbg_coll(NULL, PARSING, FLUSH_ONE), 0);
        }
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
	return (exec_non_builtin_solo(node));
}
