/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/09 17:31:49 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/19 08:41:21 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	env_lst_size(t_env **lst)
{
	t_env	*node;
	int		size;

	node = *lst;
	size = 0;
	while (node)
	{
		size++;
		node = node->next;
	}
	return (size);
}

void	lst_env_add_back(t_env **lst, t_env *new)
{
	t_env	*list;

	if (!*lst)
	{	
		// printf("IT'S EMPTY\n");
		*lst = new; // Handling an empty f*ing list
	}
	else
	{
		// printf("IT'S not EMPTY\n");
		list = *lst;
		while (list->next)
			list = list->next;
		list->next = new;
	}
	
	// printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
	// printf("PRINTING THE ENV AFTER APPENDING\n");
	// print_env(lst);
	// printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

char	**env_lst_to_arr(t_env **lst)
{
	char	**env_arr;
	t_env	*node;
	int		i;

	node = *lst;
	env_arr = malloc(sizeof(char *) * (env_lst_size(lst) + 1));
	if (!env_arr || gbg_coll(env_arr, ENV, ADD) == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	i = 0;
	while (node)
	{
		env_arr[i] = ft_strjoin(node->field, node->content);
		if (!env_arr[i] || gbg_coll(env_arr[i], ENV, ADD) == -1)
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		node = node->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

t_env	*create_env_node(char *str, char *sep)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node || gbg_coll(new_node, ENV, ADD) == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		
	new_node->field = malloc(sizeof(char) * (ft_strlen_sep(str, sep) + 1));
	if (!new_node->field || gbg_coll(new_node->field, ENV, ADD) == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(new_node->field, str, ft_strlen_sep(str, sep) + 1);
	
	new_node->content = malloc(sizeof(char) * (ft_strlen(sep + 1) + 1));
	if (!new_node->content || gbg_coll(new_node->content, ENV, ADD) == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(new_node->content, sep + 1, ft_strlen(sep));
	
	new_node->next = NULL;
	return (new_node);
}

t_env	*get_env_lst(char **envp)
{
	t_env	*lst;
	t_env	*node;
	char	*sep;
	int		i;

	i = -1;
	while (envp && envp[++i])
	{
		sep = ft_strchr(envp[i], '=');
		node = create_env_node(envp[i], sep);
		if (!node)
			return (NULL);
		if (i == 0)
		{
			lst = node;
			continue ;
		}
		else
			lst_env_add_back(&lst, node);
	}
	return (lst);
}
