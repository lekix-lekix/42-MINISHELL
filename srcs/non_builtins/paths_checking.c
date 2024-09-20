/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_checking.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 07:19:27 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/20 17:47:37 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**ft_get_paths(char **env)
{
	char	**path;
	int		x;

	x = 0;
	while (env[x])
	{
		if (ft_strncmp("PATH", env[x], 4) == 0)
		{
			path = msh_split(env[x] + 5, ':', PARSING);
			return (path);
		}
		x++;
	}
	return (NULL);
}

int	is_abs_path(char *str)
{
	if (str && str[0] && str[0] == '.' && str[1] && str[1] == '/')
		return (1);
	else if (str && str[0] && str[0] == '/')
		return (1);
	return (0);
}

char	*check_cmd_access(char *cmd)
{
	struct stat	file_stat;
	int			err;

	err = access(cmd, F_OK | R_OK | X_OK);
	if (err == 0)
	{
		stat(cmd, &file_stat);
		if (S_ISDIR(file_stat.st_mode) && is_abs_path(cmd))
			return (print_msh_error(IS_DIR_ERR, cmd), ft_exit_close(126), NULL);
		else
			return (cmd);
	}
	else if (err == -1 && errno == EACCES && is_abs_path(cmd))
		return (print_msh_error(PER_ERR, cmd), ft_exit_close(126), NULL);
	else if (err == -1 && is_abs_path(cmd))
		return (print_msh_error(NOT_FOUND_ERR, cmd), ft_exit_close(127), NULL);
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
		if (access(fpath, F_OK | R_OK | X_OK) == 0)
			return (fpath);
	}
	return (print_msh_error(CMD_ERR, cmd), ft_exit_close(127), NULL);
}

char	*ft_check_path(char **contents, char **env, int *exit_status, int *i)
{
	char	**paths;
	char	*fpath;
	char	*cmd;

	cmd = contents[*i];
	if (!contents || !cmd)
		return (*exit_status = 0, NULL);
	if (!cmd[0])
	{
		*i = *i + 1;
		return (ft_check_path(contents, env, exit_status, i));
	}
	if (cmd[0] == '.' && ft_strlen(cmd) == 1)
		return (dot_error());
	fpath = check_cmd_access(cmd);
	if (fpath != NULL)
		return (cmd);
	paths = ft_get_paths(env);
	if (!paths || paths[0] == NULL)
		return (NULL);
	fpath = check_paths(paths, cmd);
	if (fpath != NULL)
		return (fpath);
	return (ft_free(paths), NULL);
}
