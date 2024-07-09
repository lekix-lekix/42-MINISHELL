/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 04:49:38 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/09 17:36:12 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

// int	ft_exec_non_builtins(char **args, t_redir *redirections)
// {
// 	char	*la_path;
// 	// char	**env;
// 	pid_t	la_fork;
// 	int		la_status;

// 	// int x = -1;
// 	// while (args[++x])
// 	printf("The built in cmd: %s\n", args[0]);
// 	// env = env_lst_to_arr(&(ft_shell())->env_lst);
// 	la_fork = fork();
// 	if (la_fork == 0)
// 	{
// 		la_status = ft_check_redirections(redirections);
// 		if (la_status != ENO_SUCCESS)
// 			(exit(ENO_GENERAL));
// 		if (args == NULL || args[0] == NULL)
// 			(ft_free(args));
// 		la_path = ft_check_path(args[0], (ft_shell())->env_args);
// 		if (!la_path)
// 			(ft_free(args), free(la_path), ft_print_err(CMD_ERR), exit(127));
// 		// execve(la_path, args, env);
// 		// (ft_free(args), ft_print_err(CMD_ERR), exit(127));
// 		dprintf(2, "LAUNCHING EXECVE CMD %s\n", args[0]);
// 		execve(la_path, args, (ft_shell())->env_args);
// 		(ft_free(args), ft_print_err(CMD_ERR), exit(127));
// 	}
// 	else
// 		waitpid(la_fork, &la_status, 0);
// 	if (WIFEXITED(la_status))
// 		return (WEXITSTATUS(la_status));
// 	else if (WIFSIGNALED(la_status))
// 		return (WTERMSIG(la_status));
// 	return (la_status);
// }

int	ft_exec_non_builtins(char **args, t_redir *redirections)
{
	int		la_status;
	char	*la_path;
	char	**env;

	env = env_lst_to_arr(&(ft_shell())->env_lst);
	la_path = ft_check_path(args[0], (ft_shell())->env_args);
	if (!la_path)
		return (ft_print_err(CMD_ERR), -1);
	la_status = ft_check_redirections(redirections);
	if (la_status != ENO_SUCCESS)
		return (la_status);
	// dprintf(2, "LAUNCHING EXECVE CMD %s\n", args[0]);
	if (execve(la_path, args, (ft_shell())->env_args) == -1)
		return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL),
			exit(255), -1);
	return (-1);
}

int	ft_exec_non_builtins_single_cmd(char **args, t_redir *redirections)
{
	char	*la_path;
	pid_t	la_fork;
	int		la_status;

	// char	**env;
	// int x = -1;
	// while (args[++x])
	printf("The cmd: %s\n", args[0]);
	// env = env_lst_to_arr(&(ft_shell())->env_lst);
	la_fork = fork();
	if (la_fork == 0)
	{
		la_status = ft_check_redirections(redirections);
		if (la_status != ENO_SUCCESS)
			(exit(ENO_GENERAL));
		if (args == NULL || args[0] == NULL)
			(ft_free(args));
		la_path = ft_check_path(args[0], (ft_shell())->env_args);
		if (!la_path)
			(ft_free(args), free(la_path), ft_print_err(CMD_ERR), exit(127));
		// execve(la_path, args, env);
		// (ft_free(args), ft_print_err(CMD_ERR), exit(127));
		execve(la_path, args, (ft_shell())->env_args);
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
