/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 04:49:38 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/02 14:38:09 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_exec_non_builtins(t_token *node)
{
	char	*la_path;

	la_path = ft_check_path(node->contents[0], ft_shell()->env_args);
	if (!la_path)
		return (ft_print_err(CMD_ERR), -1);
	// dprintf(2, "LAUNCHING EXECVE CMD %s\n", node->contents[0]);
	if (execve(la_path, node->contents, ft_shell()->env_args) == -1)
		return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL),
			exit(255), -1);
	return (-1);
}
