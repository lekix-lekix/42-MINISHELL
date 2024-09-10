/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 04:49:38 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/10 22:36:39 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int    ft_exit_close(int exit_status)
{
    close(ft_shell()->ft_stdin);
    close(ft_shell()->ft_stdout);
    return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(exit_status), -1);
}

int    ft_exec_non_builtins(t_token *node)
{
	char	*la_path;
	char	**env_args;
	int		exit_status;
	int		i;

	i = 0;
	env_args = env_lst_to_arr(&ft_shell()->env_lst);
	la_path = ft_check_path(node->contents, env_args, &exit_status, &i);
	// dprintf(2, "launching cmd %s\n", node->contents[0]);
	close(ft_shell()->ft_stdin);
	close(ft_shell()->ft_stdout);
	// dprintf(2, "launching cmd %s\n", node->contents[0]);
	if (execve(la_path, node->contents + i, ft_shell()->env_args) == -1)
		return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL), exit(exit_status), -1);
	return (0);
}
