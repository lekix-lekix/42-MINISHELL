/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 07:19:27 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/22 18:18:02 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**ft_get_paths(char **env);
char	*ft_join(char *s1, char *s2);
char	*check_cmd_access(char *cmd);
char	*check_paths(char **paths, char *cmd);
char	*check_cmd_path(char *cmd);

char	*ft_check_path(char *cmd, char **env)
{
	char	**paths;
	char	*fpath;

	fpath = check_cmd_access(cmd);
	if (fpath != NULL)
		return (fpath);
	fpath = check_cmd_path(cmd);
	if (fpath != NULL)
		return (fpath);
	paths = ft_get_paths(env);
	if (!paths || paths[0] == NULL)
		return (cmd);
	fpath = check_paths(paths, cmd);
	if (fpath != NULL)
		return (fpath);
	return (ft_free(paths), NULL);
}

char	**ft_get_paths(char **env)
{
	char	**path;
	int		x;

	x = 0;
	while (env[x])
	{
		if (ft_strncmp("PATH", env[x], 4) == 0)
		{
			path = ft_split(env[x] + 5, ':');
			return (path);
		}
		x++;
	}
	return (NULL);
}

char	*check_cmd_access(char *cmd)
{
	if (access(cmd, F_OK | R_OK | X_OK) == 0)
		return (cmd);
	return (NULL);
}

char	*check_cmd_path(char *cmd)
{
	if (ft_strncmp(cmd, "/", 5) == 0)
	{
		if (access(cmd, F_OK | R_OK | X_OK) == 0)
			return (cmd);
		(ft_print_err("Error in the command path"));
	}
	return (NULL);
}

char	*check_paths(char **paths, char *cmd)
{
	char	*fpath;
	char	*tmp;
	int		x;

	x = -1;
	while (paths && paths[++x])
	{
		tmp = ft_join(paths[x], "/");
		fpath = ft_join(tmp, cmd);
		free(tmp);
		if (!fpath)
			return (ft_free(paths), NULL);
		if (access(fpath, F_OK | R_OK | X_OK) == 0)
			return (ft_free(paths), fpath);
		(free(fpath), fpath = NULL);
	}
	return (NULL);
}
