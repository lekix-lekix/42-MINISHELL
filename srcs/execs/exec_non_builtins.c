/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 04:49:38 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/20 15:50:24 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_exec_non_builtins(t_token *node)
{
	char	*la_path;
	char	**env_args;
	
	ft_shell()->exit_status = ft_check_redirections(node);
	env_args = env_lst_to_arr(&ft_shell()->env_lst);
	la_path = ft_check_path(node->contents[0], env_args);
	if (!la_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(node->contents[0], 2);
		// ft_putchar_fd(node->contents[1], 2);
		ft_putstr_fd(" :", 2);
		return (ft_print_err(CMD_ERR), -1);
  }
	// if (execve(la_path, node->contents, ft_shell()->env_args) == -1)
		//return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL),
	// dprintf(2, "LAUNCHING EXECVE CMD %s\n", node->contents[0]);
	if (execve(la_path, node->contents, env_args) == -1)
		return (perror("bash: execve: "), ft_free(env_args),
			exit(255), -1);

	// return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL),
	// 	exit(255), -1);
	return (-1);
}
