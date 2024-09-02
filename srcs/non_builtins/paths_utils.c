/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paths_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 07:19:27 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/02 13:50:16 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	**ft_get_paths(char **env);
char	*ft_join(char *s1, char *s2);
char	*check_cmd_access(char *cmd, int *exit_status);
char	*check_paths(char **paths, char *cmd, int *exit_status);
char	*check_cmd_path(char *cmd);

int	print_msh_error(char *err, char *cmd)
{
	char	*str;

	str = ft_join("minishell: ", cmd);
	str = ft_join(str, ": ");
	str = ft_join(str, err);
	str = ft_join(str, "\n");
	if (!str)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	write(2, str, ft_strlen(str));
	return (0);
}

// char	*ft_check_path(char **contents, char **env, int *exit_status, int *i)
// {
// 	char	**paths;
// 	char	*fpath;
// 	char	*cmd;

//     printf("i = %d\n", *i);
// 	cmd = contents[*i];
// 	if (!contents || !cmd)
// 		return (*exit_status = 0, NULL);
// 	if (cmd[0])
//     {
//         *i = *i + 1;
// 		return (ft_check_path(contents, env, exit_status, i));
//     }
// 	fpath = check_cmd_access(cmd, exit_status);
// 	if (!fpath)
// 		return (NULL);
// 	paths = ft_get_paths(env);
// 	if (!paths || paths[0] == NULL)
// 		return (NULL);
// 	fpath = check_paths(paths, cmd, exit_status);
// 	if (fpath != NULL)
// 		return (fpath);
// 	return (ft_free(paths), NULL);
// }


char	*ft_check_path(char *cmd, char **env, int *exit_status)
{
	char	**paths;
	char	*fpath;

	if (!cmd || !cmd[0])
		return (print_msh_error(CMD_ERR, cmd), *exit_status = 127, NULL);
	fpath = check_cmd_access(cmd, exit_status);
	if (!fpath)
		return (NULL);
	paths = ft_get_paths(env);
	if (!paths || paths[0] == NULL)
		return (NULL);
	fpath = check_paths(paths, cmd, exit_status);
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
			if (!path)
				return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
			return (path);
		}
		x++;
	}
	return (NULL);
}

char	*check_cmd_access(char *cmd, int *exit_status)
{
	struct stat	file_stat;
	int			err;

	err = access(cmd, F_OK | R_OK | X_OK);
	if (err == 0)
	{
		stat(cmd, &file_stat);
		if (S_ISDIR(file_stat.st_mode))
			return (print_msh_error(IS_DIR_ERR, cmd), *exit_status = 126, NULL);
	}
	else if (err == -1 && errno == EACCES)
		return (print_msh_error(PER_ERR, cmd), *exit_status = 127, NULL);
	return (cmd);
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

char	*check_paths(char **paths, char *cmd, int *exit_status)
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
	return (print_msh_error(CMD_ERR, cmd), *exit_status = 127, NULL);
}
