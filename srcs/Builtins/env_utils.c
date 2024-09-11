/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:12:40 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 15:08:34 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_env	*ft_envlst_new(char *field, char *content)
{
	t_env	*new;

	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!new || gbg_coll(new, ENV, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	new->field = msh_strdup(field, ENV);
	if (content && content[0])
		new->content = msh_strdup(content, ENV);
	else
		new->content = NULL;
	new->next = NULL;
	return (new);
}

char	*ft_get_envlst_content(char *key, t_env **env_lst)
{
	t_env	*envlst;

	envlst = *env_lst;
	// while (1)
	// {
		while (envlst)
		{
			if (!ft_strcmp(key, envlst->field))
				return (envlst->content);
			envlst = envlst->next;
		}
		// envlst = *env_lst;
		// if (!key[0] || ft_strlen(key) == 1)
			// // break ;
		// // key = msh_strtrim(key, &key[ft_strlen(key) - 1]);
        // // ft_shell()->expand_chars_trimmed += 1;
	// }
    // ft_shell()->expand_chars_trimmed = 0;
	return (NULL);
}

static int	ft_loop_and_update(t_env **to_update_lst, char *content,
		char *field)
{
	t_env	*envlst;

	envlst = *to_update_lst;
	while (envlst)
	{
		if (envlst->field && !ft_strcmp(envlst->field, field))
		{
			if (content)
				envlst->content = msh_strdup(content, ENV);
			return (1);
		}
		envlst = envlst->next;
	}
	return (0);
}

char	*get_env_content(char *field)
{
	t_env	*envlst;

	envlst = ft_shell()->env_lst;
	while (envlst)
	{
		if (envlst->field && !ft_strcmp(envlst->field, field)
			&& envlst->content)
			return (envlst->content);
		envlst = envlst->next;
	}
	return (NULL);
}

void	ft_update_envlst(char *field, char *content, bool create)
{
	t_env	*le_env;
	t_env	*envlst;

	envlst = ft_shell()->expanded_env;
	le_env = ft_shell()->env_lst;
	if (ft_loop_and_update(&le_env, content, field)
		&& ft_loop_and_update(&envlst, content, field))
		return ;
	if (create)
	{
		lst_env_add_back(&envlst, ft_envlst_new(field, content));
		if (content)
			lst_env_add_back(&le_env, ft_envlst_new(field, content));
	}
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
				return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(),
					exit(255), NULL);
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
				return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(),
					exit(255), NULL);
			return (final_str);
		}
		i++;
	}
	return (NULL);
}
