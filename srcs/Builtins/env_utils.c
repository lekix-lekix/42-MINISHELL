/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 15:12:40 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/21 12:34:04 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static t_env	*ft_envlst_new(char *field, char *content)
{
	t_env	*new;

	new = (t_env *)ft_calloc(1, sizeof(t_env));
	if (!new || gbg_coll(new, ENV, ADD))
		return (ft_exit_close(255), NULL);
	new->field = msh_strdup(field, ENV);
	if (content && content[0])
		new->content = msh_strdup(content, ENV);
	else
		new->content = NULL;
	new->next = NULL;
	return (new);
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

void	ft_update_envlst(char *field, char *content, t_env **lst, bool create)
{
	if (ft_loop_and_update(lst, content, field))
		return ;
	if (create)
		lst_env_add_back(lst, ft_envlst_new(field, content));
}
