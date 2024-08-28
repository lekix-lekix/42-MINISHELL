/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:20:35 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/26 16:46:05 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_export_err_msg(char *identifier)
{
	ft_putstr_fd("minishell: export: `", 2);
	if (identifier[0] == '-')
	{
		ft_putchar_fd(identifier[0], 2);
		ft_putchar_fd(identifier[1], 2);
		ft_putstr_fd("': invalid option\n", 2);
	}
	return (1);
}

int	ft_exec_pwd(char **args)
{
	char	*cwd;

	cwd = NULL;
	if (args[1] && args[1][0] == '-')
		return (ft_export_err_msg(args[1]));
	cwd = getcwd(cwd, 0);
	if (!cwd)
		return (1);
	ft_putstr_fd(cwd, 1);
	ft_putstr_fd("\n", 1);
	free(cwd);
	return (0);
}
