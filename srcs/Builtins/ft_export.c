/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:02:12 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/18 14:14:08 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_export_err_msg(char *identifier)
{
	ft_putstr_fd("minishell: export: `", 2);
	ft_putstr_fd(identifier, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static void	ft_export_list(t_minishell *data)
{
	t_env	*list;
	size_t	i;

	list = data->env_lst;
	while (list)
	{
		if (list->field != NULL && (ft_strcmp(list->content, "_") != 0))
		{
			printf("export %s=\"", list->content);
			i = 0;
			while ((list->field)[i])
			{
				if ((list->field)[i] == '$' || (list->field)[i] == '"')
					printf("\\%c", (list->field)[i++]);
				else
					printf("%c", (list->field)[i++]);
			}
			printf("\"\n");
		}
		else if (list->field == NULL && (ft_strcmp(list->content, "_") != 0))
			printf("export %s\n", list->content);
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

int	ft_exec_export(char **args, t_minishell *data)
{
	int		i;
	int		exit_s;
	char	*key;

	exit_s = 0;
	i = 1;
	if (!args[1])
		return (ft_export_list(data), 0);
	while (args[i])
	{
		if (ft_check_key(args[i]) == 0)
			exit_s = ft_export_err_msg(args[i]);
		else
		{
			key = ft_extract_val(args[i]);
			if (ft_env_entry_exists(key, data))
				ft_update_envlst(ft_extract_key(args[i]), key, false, data);
			else
				ft_update_envlst(ft_extract_key(args[i]), key, true, data);
		}
		i++;
	}
	return (exit_s);
}
// printf("The field: %s\n", key);
// printf("The content: %s\n", ft_extract_key(args[i]));
