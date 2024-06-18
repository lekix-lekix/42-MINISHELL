/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:06:23 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/17 12:02:10 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_unset_helper(char *key, t_minishell *data)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = data->env_lst;
	while (current)
	{
		if (!ft_strncmp(key, current->field, ft_strlen(key)))
		{
			if (prev)
				prev->next = current->next;
			else
				data->env_lst = current->next;
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	ft_exec_unset(char **args, t_minishell *data)
{
	int		i;
	bool	err;

	i = 1;
	if (!args[1])
		return (0);
	err = false;
	while (args[i])
	{
		if (!ft_check_key(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			err = true;
		}
		else
		{
			printf("The extracted key: %s\n", ft_extract_val(args[i]));
			ft_unset_helper(ft_extract_val(args[i]), data);
		}
		i++;
	}
	return (err);
}
