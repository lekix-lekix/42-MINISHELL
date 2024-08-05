/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:03 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/05 06:14:32 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_strncmp_loco(const char *s1, const char *s2, size_t n)
{
	unsigned int	i;

	i = 0;
	if (n == 0)
		return (0);
	// printf("THE LENG IS: %zu\n", n);
	// printf("THE S1: %s\n", s1);
	// printf("THE S2: %s\n", s2);
	while ((i < n - 1) && ((unsigned char)s1[i] != '\0'
			|| (unsigned char)s2[i] != '\0'))
	{
		// printf("IT DOESN'T GETS HERE29\n");
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
	if (ft_strncmp_loco(args[0], "export", ft_strlen(args[0])) == 0)
		return (ft_exec_export(args));
	if (ft_strncmp_loco(args[0], "unset", ft_strlen("unset")) == 0)
		return (ft_exec_unset(args, data));
	if (ft_strncmp_loco(args[0], "cd", ft_strlen("cd")) == 0)
		return (ft_do_cd(args, data));
	return (ENO_GENERAL);
}

bool	ft_is_builtin(char *arg)
{
	// dprintf(2, "arg = %s\n", arg);
	if (!ft_strncmp_loco(arg, "cd", ft_strlen(arg)) || !ft_strncmp_loco(arg,
			"pwd", ft_strlen(arg)) || !ft_strncmp_loco("echo", arg,
			ft_strlen(arg)) || !ft_strncmp_loco(arg, "export", ft_strlen(arg))
		|| !ft_strncmp_loco(arg, "env", ft_strlen(arg)) || !ft_strncmp_loco(arg,
			"unset", ft_strlen(arg)))
		return (true);
	else
		return (false);
}
