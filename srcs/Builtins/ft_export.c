/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:02:12 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/06 15:22:45 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_export_err_msg(char *identifier)
{
	ft_putstr_fd("minishell: export: `", 2);
	if (identifier[0] == '-')
	{
		ft_print_err(identifier);
		ft_putstr_fd("': invalid option\n", 2);
		return (2);
	}
	ft_putstr_fd(identifier, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
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

static void	ft_export_list(t_env **envlst)
{
	t_env	*list;

	// size_t	i;
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

int	ft_check_key(char *str)
{
	int	i;

	i = 1;
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_exec_export(char **args)
{
	int		i;
	int		exit_s;
	char	*field;
	t_env	*envlst;    

	exit_s = 0;
	i = 1;
	envlst = ft_shell()->env_lst;
	if (!args[1] || args[1][0] == '$')
		return (ft_export_list(&envlst), 0);
	while (args[i])
	{
		if (ft_check_key(args[i]) == 0)
			exit_s = ft_export_err_msg(args[i]);
		else
		{
			field = ft_extract_field(args[i]);
			if (ft_env_entry_exists(field))
				ft_update_envlst(field, ft_extract_content(args[i]), false);
			else
				ft_update_envlst(field, ft_extract_content(args[i]), true);
		}
		i++;
	}
	return (exit_s);
}
