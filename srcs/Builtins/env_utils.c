/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:12:40 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/23 17:36:02 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_env	*ft_envlst_new(char *content, char *field)
{
	t_env	*new;

	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!new || gbg_coll(new, ENV, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	new->content = msh_strdup(content, ENV);
	if (field)
	{
		new->content = msh_strdup(field, ENV);
		new->field = msh_strdup(content, ENV);
	}
	new->next = NULL;
	return (new);
}

char	*ft_get_envlst_content(char *key, t_minishell *data)
{
	t_env	*envlst;

	envlst = data->env_lst;
	while (envlst)
	{
		// I'm using a function that I shuldn't use here,
		// don't delete this comment unless you change this function
		if (!ft_strncmp_loco(key, envlst->field, ft_strlen(key)))
			return (envlst->content);
		envlst = envlst->next;
	}
	return (NULL);
}

static int	ft_loop_and_update(t_env **to_update_lst, char *value, char *key)
{
	t_env	*envlst;
	int		key_len;

	envlst = *to_update_lst;
	key_len = ft_strlen(key);
	while (envlst)
	{
		if (envlst->field && !ft_strncmp(envlst->field, key, key_len))
		{
			if (value)
				envlst->content = msh_strdup(value, ENV);
			return (1);
		}
		envlst = envlst->next;
	}
	return (0);
}

void	ft_update_envlst(char *key, char *value, bool create)
{
	t_env	*le_env;
	t_env	*envlst;

	envlst = ft_shell()->expanded_env;
	le_env = ft_shell()->env_lst;
	if (ft_loop_and_update(&le_env, value, key) && ft_loop_and_update(&envlst,
			value, key))
		return ;
	if (create)
	{
		lst_env_add_back(&envlst, ft_envlst_new(key, value));
		if (value)
			lst_env_add_back(&le_env, ft_envlst_new(key, value));
	}
	// print_env(&le_env);
	// print_env(&envlst);
}

char	*ft_extract_val(char *str)
{
	size_t	i;
	char	*final_str;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
		{
			final_str = ft_substr(final_str, 0, i + 1);
			if (!final_str || gbg_coll(final_str, ENV, ADD))
				return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
			return (final_str);
		}
		i++;
	}
	final_str = ft_strdup(str);
	if (!final_str || gbg_coll(final_str, ENV, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (final_str);
}

char	*ft_extract_key(char *str)
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
                return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
			return (final_str);
		}
		i++;
	}
	return (NULL);
}
    