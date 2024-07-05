/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:02:12 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/05 13:35:32 by sabakar-         ###   ########.fr       */
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
		if (list->content != NULL && (ft_strcmp(list->field, "_") != 0))
		{
			printf("export %s\"", list->field);
			i = 0;
			while ((list->content)[i])
			{
				if ((list->content)[i] == '$' || (list->content)[i] == '"')
					printf("\\%c", (list->content)[i++]);
				else
					printf("%c", (list->content)[i++]);
			}
			printf("\"\n");
		}
		else if (list->content == NULL && (ft_strcmp(list->field, "_") != 0))
			printf("export F %s\n", list->field);
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
	dprintf(2, "The cmd is: %s\n", args[0]);	
	dprintf(2, "The cmd is: %s\n", args[1]);
	if (!args[1])
		return (ft_export_list(data), 0);
	while (args[i])
	{
		if (ft_check_key(args[i]) == 0)
			exit_s = ft_export_err_msg(args[i]);
		else
		{
			key = ft_extract_val(args[i]);
			printf("The key: %s\n", key);
			printf("The key of: %s\n", (ft_shell())->env_lst->field);
			if (ft_env_entry_exists(key, ft_shell()))
			{	
				printf("The crash\n");
				ft_update_envlst(ft_extract_key(args[i]), key, false, data);
			}
			else
				ft_update_envlst(ft_extract_key(args[i]), key, true, data);
		}
		i++;
	}
	return (exit_s);
}
// printf("The field: %s\n", key);
// printf("The content: %s\n", ft_extract_key(args[i]));
