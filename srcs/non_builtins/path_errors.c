/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:34:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/21 12:52:04 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_msh_error(char *err, char *cmd)
{
	char	*str;

	str = ft_join("minishell: ", cmd);
	str = ft_join(str, ": ");
	str = ft_join(str, err);
	str = ft_join(str, "\n");
	write(2, str, ft_strlen(str));
	return (0);
}

void	check_dot_errors(char *cmd)
{
	if (cmd[0] == '.' && ft_strlen(cmd) == 1)
	{
		write(2, "bash: .: filename argument required\n", 37);
		write(2, ".: usage: . filename [arguments]\n", 34);
		ft_exit_close(2);
	}
	if (cmd[0] == '.' && cmd[1] == '.' && ft_strlen(cmd) == 2)
	{
		print_msh_error(CMD_ERR, cmd);
		ft_exit_close(127);
	}
}
