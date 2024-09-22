/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 18:02:12 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/22 17:22:33 by lekix            ###   ########.fr       */
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

int	ft_check_key(char *str)
{
	int	i;

	i = 1;
	if (!ft_isalpha(*str) && *str != '_')
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (1);
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	check_and_update_envlst(char **args, int *exit_s, int i)
{
	char	*field;
	char	*content;

	content = ft_extract_content(args[i]);
	field = ft_extract_field(args[i]);
	if (check_export_concat(field, args))
		return (*exit_s);
	if (ft_env_entry_exists(field, &ft_shell()->env_lst))
		ft_update_envlst(field, content, &ft_shell()->env_lst, false);
	else
		ft_update_envlst(field, content, &ft_shell()->env_lst, true);
	if (ft_env_entry_exists(field, &ft_shell()->expanded_env))
		ft_update_envlst(field, content, &ft_shell()->expanded_env, false);
	else if (!ft_env_entry_exists(field, &ft_shell()->expanded_env) && content)
		ft_update_envlst(field, content, &ft_shell()->expanded_env, true);
	return (0);
}

int	ft_exec_export(char **args)
{
	int		exit_s;
	t_env	*envlst;
	int		i;

	i = 0;
	exit_s = 0;
	envlst = ft_shell()->env_lst;
	if (!args[1] || args[1][0] == '$')
		return (ft_export_list(&envlst), 0);
	while (args[++i])
	{
		if (ft_check_key(args[i]) == 0)
			exit_s = ft_export_err_msg(args[i]);
		else
			check_and_update_envlst(args, &exit_s, i);
	}
	return (exit_s);
}
