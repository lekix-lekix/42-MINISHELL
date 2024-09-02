/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 14:44:43 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/02 17:44:49 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_cdhome(t_minishell *data);
static int	ft_change_cwd(void);
// static int	ft_cderr_msg(char *err_msg);

static int	ft_change_cwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd || gbg_coll(cwd, ENV, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	return (ft_update_envlst("PWD", cwd, false), 0);
}

static int	ft_cdhome(t_minishell *data)
{
	char	*home;

	ft_update_envlst("OLDPWD", ft_get_envlst_content("PWD", data), false);
	home = ft_get_envlst_content("HOME", data);
	if (!home)
		return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	if (chdir(home) == ENO_SUCCESS)
		return (ft_update_envlst("PWD", home, false), 0);
	return (1);
}

// static int	ft_cderr_msg(char *err_msg)
// {
// 	ft_putstr_fd("minishell: cd: `", 2);
// 	ft_putstr_fd(err_msg, 2);
// 	ft_putstr_fd("' can't cd!\n", 2);
// 	return (1);
// }

int	ft_do_cd(char **path, t_minishell *data)
{
	if (!path[1])
		return (ft_cdhome(data));
    if (get_arr_len(path) > 2)
    {
        write(2, "minishell: cd: too many arguments\n", 35);
        return (1);
    }
	if (chdir(path[1]) != ENO_SUCCESS)
    {
        perror("minishell: cd");
		return (1);
    }
	ft_update_envlst("OLDPWD", ft_get_envlst_content("PWD", data), false);
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
