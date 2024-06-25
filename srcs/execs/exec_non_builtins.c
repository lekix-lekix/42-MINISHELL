/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 04:49:38 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/25 15:21:06 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_exec_non_builtins(char **args, t_minishell *data, t_redir *redirections)
{
	char	*la_path;
	char	**env;
	pid_t	la_fork;
	int		la_status;

	la_fork = fork();
	if (la_fork == 0)
	{
		la_status = ft_check_redirections(redirections, data);
		if (la_status != ENO_SUCCESS)
			(exit(ENO_GENERAL));
		env = env_lst_to_arr(&data->env_lst);
		if (args == NULL || args[0] == NULL)
			(ft_free(args));
		la_path = ft_check_path(args[0], env);
		if (!la_path)
			(ft_free(args), free(la_path), ft_print_err(CMD_ERR), exit(127));
		execve(la_path, args, env);
		(ft_free(args), ft_print_err(CMD_ERR), exit(127));
	}
	else
		waitpid(la_fork, &la_status, 0);
	if (WIFEXITED(la_status))
		return (WEXITSTATUS(la_status));
	else if (WIFSIGNALED(la_status))
		return (WTERMSIG(la_status));
	return (la_status);
}
