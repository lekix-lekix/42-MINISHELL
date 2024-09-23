/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 09:06:23 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/21 11:11:25 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_unset_helper(char *field, t_env **envlst)
{
	t_env	*current;
	t_env	*prev;

	prev = NULL;
	current = *envlst;
	while (current)
	{
		if (!ft_strcmp(field, current->field))
		{
			if (prev)
				prev->next = current->next;
			else
			{
				ft_shell()->expanded_env = current->next;
				ft_shell()->env_lst = current->next;
			}
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

int	ft_exec_unset(char **args)
{
	int		i;
	int		f;
	bool	err;
	t_env	*le_env;
	t_env	*envlst;

	i = 0;
	f = 0;
	le_env = (ft_shell())->env_lst;
	envlst = (ft_shell())->expanded_env;
	if (!args[1])
		return (0);
	err = false;
	while (args[++i])
		ft_unset_helper(ft_extract_field(args[i]), &envlst);
	while (args[++f])
		ft_unset_helper(ft_extract_field(args[f]), &le_env);
	return (err);
}
