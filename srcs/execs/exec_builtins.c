/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:03 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/29 15:25:07 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_strncmp_loco(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	while ((i < n - 1) && ((unsigned char)s1[i] != '\0'
			|| (unsigned char)s2[i] != '\0'))
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	ft_exec_builtins(char **args, t_minishell *data)
{	
	if (ft_strncmp_loco(args[0], "echo", ft_strlen(args[0])) == 0)
		return (ft_exec_echo(args));
	if (ft_strncmp_loco(args[0], "pwd", ft_strlen(args[0])) == 0)
		return (ft_exec_pwd());
	if (ft_strncmp_loco(args[0], "env", ft_strlen(args[0])) == 0)
		return (print_env(&data->env_lst));
	if (ft_strncmp_loco(args[0], "export", ft_strlen("export")) == 0)
		return (ft_exec_export(args, data));
	if (ft_strncmp_loco(args[0], "unset", ft_strlen("unset")) == 0)
		return (ft_exec_unset(args, data));
	if (ft_strncmp_loco(args[0], "cd", ft_strlen("cd")) == 0)
		return (ft_do_cd(args, data));
	return (ENO_GENERAL);
}

bool	ft_is_builtin(char *arg)
{
	if (!ft_strncmp_loco(arg, "cd", ft_strlen(arg)) || !ft_strncmp_loco(arg,
			"pwd", ft_strlen(arg)) || !ft_strncmp_loco("echo", arg,
			ft_strlen(arg)) || !ft_strncmp_loco(arg, "export", ft_strlen(arg))
		|| !ft_strncmp_loco(arg, "env", ft_strlen(arg)) || !ft_strncmp_loco(arg,
			"unset", ft_strlen(arg)))
		return (true);
	else
		return (false);
}
