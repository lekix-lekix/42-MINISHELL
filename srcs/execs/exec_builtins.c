/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_builtins.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 11:31:03 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/09 20:21:03 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int	ft_strncmp_loco(const char *s1, const char *s2, size_t n)
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

int ft_env(char **args)
{
    char *err;
    
    if (get_arr_len(args) > 1)
    {
        err = ft_join("env: ‘", args[1]);
        err = ft_join(err, "’: No such file or directory\n");
        write(2, err, ft_strlen(err));
        return (127);
    }
    print_env(&ft_shell()->env_lst);
    return (0);
}

int	ft_exec_builtins(char **args)
{
	if (ft_strcmp(args[0], "echo") == 0)
		return (ft_exec_echo(args));
	if (ft_strcmp(args[0], "pwd") == 0)
		return (ft_exec_pwd(args));
	if (ft_strcmp(args[0], "env") == 0)
		return (ft_env(args));
	if (ft_strcmp(args[0], "export") == 0)
		return (ft_exec_export(args));
	if (ft_strcmp(args[0], "unset") == 0)
		return (ft_exec_unset(args));
	if (ft_strcmp(args[0], "cd") == 0)
		return (ft_do_cd(args));
	ft_exit(args);
	return (ENO_GENERAL);
}

bool	ft_is_builtin(char *arg)
{
	if (!ft_strcmp(arg, "cd") || !ft_strcmp(arg, "pwd") || !ft_strcmp("echo",
			arg) || !ft_strcmp(arg, "export") || !ft_strcmp(arg, "env")
		|| !ft_strcmp(arg, "unset") || (!ft_strcmp(arg, "exit")))
		return (true);
	else
		return (false);
}
