/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:12:40 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/18 14:13:28 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_env	*ft_envlst_new(char *content, char *field)
{
	t_env	*new;

	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!new)
		return (NULL);
	new->content = ft_strdup(content);
	if (field)
		new->field = ft_strdup(field);
	new->next = NULL;
	return (new);
}

char	*ft_get_envlst_content(char *key, t_minishell *data)
{
	t_env	*envlst;

	envlst = data->env_lst;
	while (envlst)
	{
		if (!ft_strncmp(key, envlst->field, ft_strlen(key)))
			return (envlst->content);
		envlst = envlst->next;
	}
	return (NULL);
}

void	ft_update_envlst(char *field, char *content, bool create,
		t_minishell *data)
{
	t_env	*envlst;

	envlst = data->env_lst;
	while (envlst)
	{
		if (!ft_strncmp(field, envlst->field, ft_strlen(field)))
		{
			if (content)
				envlst->content = ft_strdup(content);
			return ;
		}
		envlst = envlst->next;
	}
	if (create)
		lst_env_add_back(&data->env_lst, ft_envlst_new(field, content));
}

char	*ft_extract_val(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (ft_substr(str, 0, i + 1));
		i++;
	}
	return (ft_strdup(str));
}

char	*ft_extract_key(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			i++;
			return (ft_substr(str, i, ft_strlen(str) - i));
		}
		i++;
	}
	return (NULL);
}
