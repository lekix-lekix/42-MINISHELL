/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:44:43 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/22 15:54:14 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_change_cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd || gbg_coll(cwd, ENV, ADD))
		return (write(2, "bash: cd: ..: No such file or directory\n", 40), 1);
	ft_update_envlst("PWD", cwd, &ft_shell()->env_lst, false);
	ft_update_envlst("PWD", cwd, &ft_shell()->expanded_env, false);
	return (0);
}

static int	ft_cdhome(void)
{
	char	*home;

	ft_update_envlst("OLDPWD", ft_get_envlst_content("PWD",
			&ft_shell()->env_lst), &ft_shell()->env_lst, false);
	ft_update_envlst("OLDPWD", ft_get_envlst_content("PWD",
			&ft_shell()->expanded_env), &ft_shell()->expanded_env, false);
	home = ft_get_envlst_content("HOME", &ft_shell()->env_lst);
	if (!home)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	if (chdir(home) == ENO_SUCCESS)
	{
		ft_update_envlst("PWD", home, &ft_shell()->env_lst, false);
		ft_update_envlst("PWD", home, &ft_shell()->expanded_env, false);
		return (0);
	}
	return (1);
}

int	ft_do_cd(char **path)
{
	char	*err;

	if (!path[1])
		return (ft_cdhome());
	if (get_arr_len(path) > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 35);
		return (1);
	}
	if (chdir(path[1]) != ENO_SUCCESS)
	{
		err = ft_join("minishell: cd: ", path[1], PARSING);
		perror(err);
		return (1);
	}
	ft_update_envlst("OLDPWD", ft_get_envlst_content("PWD",
			&ft_shell()->env_lst), &ft_shell()->env_lst, false);
	ft_update_envlst("OLDPWD", ft_get_envlst_content("PWD",
			&ft_shell()->expanded_env), &ft_shell()->expanded_env, false);
	return (ft_change_cwd());
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	s1_len;
	size_t	s2_len;

	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	if (s1_len > s2_len)
		return (ft_strncmp(s1, s2, s1_len));
	else
		return (ft_strncmp(s1, s2, s2_len));
}
