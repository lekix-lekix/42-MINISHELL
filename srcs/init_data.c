/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:06:34 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/22 15:54:41 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*increment_shlvl(char *shlvl)
{
	char	*new_shlvl;
	char	*new_lvl;
	int		level;

	level = ft_atoi(shlvl + 6) + 1;
	new_lvl = ft_itoa(level);
	if (!new_lvl || gbg_coll(new_lvl, ENV, ADD))
		return (ft_exit_close(255), NULL);
	new_shlvl = ft_join("SHLVL=", new_lvl, ENV);
	return (new_shlvl);
}

int	find_increment_shlvl(char **envp)
{
	int	i;

	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], "SHLVL", 5) == 0)
			envp[i] = increment_shlvl(envp[i]);
	}
	return (0);
}

int	init_data(t_minishell *data, char **envp)
{
	data->path = get_path(envp);
	if (!data->path)
		return (-1);
	find_increment_shlvl(envp);
	data->env_lst = get_env_lst(envp);
	data->expanded_env = get_env_lst(envp);
	data->env_args = envp;
	if (!data->env_lst)
		return (-1);
	tcgetattr(STDIN_FILENO, &ft_shell()->original_term);
	return (0);
}

t_minishell	*ft_shell(void)
{
	static t_minishell	data;

	return (&data);
}
