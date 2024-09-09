/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:06:23 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/09 19:53:11 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_unset_helper(char *field)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = ft_shell()->env_lst;
	while (current)
	{
		if (!ft_strcmp(field, current->field))
		{
			if (prev)
				prev->next = current->next;
			else
				ft_shell()->env_lst = current->next;
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_check_key_unset(char *str)
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

int	ft_exec_unset(char **args)
{
	int		i;
	bool	err;

	i = 0;
	if (!args[1])
		return (0);
	err = false;
	while (args[++i])
		ft_unset_helper(ft_extract_field(args[i]));
	return (err);
}
