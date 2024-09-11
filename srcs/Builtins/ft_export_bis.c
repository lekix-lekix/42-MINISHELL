/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:59:43 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 21:06:39 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_env	*sort_envlst(t_env **envlst)
{
	t_env	*sorted_lst;
	t_env	*current;
	t_env	tmp;

	sorted_lst = env_cpy_lst(envlst);
	current = sorted_lst;
	while (current)
	{
		if (current->next && current->field && current->next->field
			&& ft_strcmp(current->field, current->next->field) > 0)
		{
			tmp.field = current->next->field;
			tmp.content = current->next->content;
			current->next->field = current->field;
			current->next->content = current->content;
			current->field = tmp.field;
			current->content = tmp.content;
			current = sorted_lst;
			continue ;
		}
		current = current->next;
	}
	return (sorted_lst);
}

void	env_lst_add_back(t_env **envlst, t_env *new_node)
{
	t_env	*current;

	current = *envlst;
	if (!current)
	{
		*envlst = new_node;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = new_node;
}

t_env	*env_cpy_lst(t_env **envlst)
{
	t_env	*current;
	t_env	*envlst_cpy;
	t_env	*new_node;

	envlst_cpy = NULL;
	current = *envlst;
	while (current)
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node || gbg_coll(new_node, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		new_node->field = current->field;
		new_node->content = current->content;
		new_node->next = NULL;
		env_lst_add_back(&envlst_cpy, new_node);
		current = current->next;
	}
	return (envlst_cpy);
}

void	ft_export_list(t_env **envlst)
{
	t_env	*list;

	list = sort_envlst(envlst);
	while (list)
	{
		if (list->content)
			printf("export %s=\"%s\"\n", list->field, list->content);
		else
			printf("export %s\n", list->field);
		list = list->next;
	}
}
