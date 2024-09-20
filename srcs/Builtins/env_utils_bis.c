/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:00:22 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/20 17:03:51 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*ft_get_envlst_content(char *key, t_env **env_lst)
{
	t_env	*envlst;

	envlst = *env_lst;
	while (envlst)
	{
		if (!ft_strcmp(key, envlst->field))
			return (envlst->content);
		envlst = envlst->next;
	}
	return (NULL);
}

char	*ft_extract_content(char *str)
{
	size_t	i;
	char	*final_str;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			i++;
			final_str = ft_substr(str, i, ft_strlen(str) - i);
			if (!final_str || gbg_coll(final_str, ENV, ADD))
				return (ft_exit_close(255), NULL);
			return (final_str);
		}
		i++;
	}
	return (NULL);
}

char	*get_path(char **envp)
{
	char	*str;
	int		i;

	i = -1;
	if (!envp || !*envp)
		return (NULL);
	while (envp && envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			str = ft_strtrim(envp[i], "PATH=");
			if (!str || gbg_coll(str, ENV, ADD))
				return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_exit_close(255),
					NULL);
		}
	}
	return (str);
}
