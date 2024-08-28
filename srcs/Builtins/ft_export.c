/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:02:12 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/28 11:54:40 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_export_err_msg(char *identifier)
{
	ft_putstr_fd("minishell: export: `", 2);
	if (identifier[0] == '-')
	{
		ft_putchar_fd(identifier[0], 2);
		ft_putchar_fd(identifier[1], 2);
		ft_putstr_fd("': invalid option\n", 2);
		return (1);
	}
	ft_putstr_fd(identifier, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	return (1);
}

static void ft_export_list(t_env **envlst) {
	t_env *list;
	size_t i;
	
    list = *envlst;
    while (list)
	{
        // printf("Current list element: %p, Field: %s\n", (void*)list, list->field ? list->field : "NULL");
		// printf("Current list element: %p, Content: %s\n", (void*)list, list->content ? list->content : "NULL");
		
		if (list->field != NULL)
		{
			printf("export %s\"", list->field ? list->field : " ");
			i = 0;
			while ((list->content) && (list->content)[i])
			{
				if ((list->content)[i] == '$' || (list->content)[i] == '"')
					printf("\\%c", (list->content)[i++]);
				else
					printf("%c", (list->content)[i++]);
			}
			printf("\"\n");
		}
		else if (list->field == NULL)
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

int	ft_exec_export(char **args)
{
	int		i;
	int		exit_s;
	char	*key;
	t_env	*envlst;
	t_env	*le_env;

	exit_s = 0;
	i = 1;
	envlst = ft_shell()->expanded_env;
	le_env = ft_shell()->env_lst;
	if (!args[1] || args[1][0] == '$')
		return ( ft_export_list(&envlst), 0);
	while (args[i])
	{
		if (ft_check_key(args[i]) == 0)
			exit_s = ft_export_err_msg(args[i]);
		else
		{
			key = ft_extract_val(args[i]);
			// printf("THE ARGS IS: %s\n", args[i]);
			if (ft_env_entry_exists(key))
				ft_update_envlst(key, ft_extract_key(args[i]), false);
			else
				ft_update_envlst(key, ft_extract_key(args[i]), true);
				// printf("WE WRE CRASHING HERE\n");
		}
		i++;
	}
	return (exit_s);
}
