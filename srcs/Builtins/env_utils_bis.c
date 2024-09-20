/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:00:22 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/19 19:11:58 by sabakar-         ###   ########.fr       */
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

char	*ft_extract_field(char *str)
{
	size_t	i;
	char	*final_str;

	i = 0;
	final_str = NULL;
	while (str[i])
	{
		if (str[i] == '=')
		{
			final_str = ft_substr(str, 0, i);
			if (!final_str || gbg_coll(final_str, ENV, ADD))
				return (ft_exit_close(255), NULL);
			return (final_str);
		}
		i++;
	}
	final_str = msh_strdup(str, ENV);
	return (final_str);
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
