/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_init_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:17:42 by lekix             #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/26 05:41:08 by sabakar-         ###   ########.fr       */
=======
/*   Updated: 2024/08/23 16:05:45 by kipouliq         ###   ########.fr       */
>>>>>>> kilian_test_more
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
		la_status = ft_exec_builtins(token_node->contents, ft_shell());
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

	if (!node->contents[0])
	{
		ft_shell()->exit_status = ft_check_redirections(node);
		return (ft_reset_ports(false), 0);
	}
	ft_check_redirections(node);
	if (ft_is_builtin(node->contents[0]))
		return (ft_exec_builtins(node->contents, ft_shell()),
			ft_reset_ports(false), 0);
	pid = fork();
	if (pid == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), perror("bash: fork"), exit(255),
			-1);
	if (pid == 0)
	{
		// dprintf(2, "WE are finished from here docs but still didn't quite\n");
		(ft_shell())->signint_child = true;
		status = ft_exec_non_builtins(node);
	}
	(ft_reset_ports(false), waitpid(pid, &status, WUNTRACED));
	if (WIFEXITED(status))
		ft_shell()->exit_status = WEXITSTATUS(status);
	return (0);
}
