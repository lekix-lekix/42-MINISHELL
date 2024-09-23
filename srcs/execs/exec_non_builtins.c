/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 04:49:38 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/23 17:27:18 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_exit_close(int exit_status)
{
	close(ft_shell()->ft_stdin);
	close(ft_shell()->ft_stdout);
	gbg_coll(NULL, ALL, FLUSH_ALL);
	rl_clear_history();
	return (exit(exit_status), -1);
}

int	no_path_found(t_token *node)
{
	close(ft_shell()->ft_stdin);
	close(ft_shell()->ft_stdout);
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(node->contents[0], 2);
	ft_putstr_fd(": ", 1);
	return (ft_print_err(CMD_ERR), ft_exit_close(127), -1);
}

int	ft_exec_non_builtins(t_token *node)
{
	char	*la_path;
	char	**env_args;
	int		exit_status;
	int		i;

	i = 0;
	env_args = env_lst_to_arr(&ft_shell()->expanded_env);
	la_path = ft_check_path(node->contents, env_args, &exit_status, &i);
	if (!la_path)
		return (no_path_found(node));
	close(ft_shell()->ft_stdin);
	close(ft_shell()->ft_stdout);
	if (execve(la_path, node->contents + i, env_args) == -1)
		return (perror("bash: execve: "), ft_exit_close(255), -1);
	return (0);
}
