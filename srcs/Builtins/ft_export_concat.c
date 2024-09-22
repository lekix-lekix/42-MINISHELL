/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_concat.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/22 16:58:23 by lekix             #+#    #+#             */
/*   Updated: 2024/09/22 17:22:52 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	create_update_both_env_lst(char *field, char *content_args)
{
	ft_update_envlst(field, content_args, &ft_shell()->env_lst, true);
	ft_update_envlst(field, content_args, &ft_shell()->expanded_env, true);
}

void	create_one_update_one_env_lst(char *field, char *content_args)
{
	char	*old_content;

	old_content = get_env_content(field, &ft_shell()->env_lst);
	if (!old_content)
		ft_update_envlst(field, content_args, &ft_shell()->env_lst, false);
	else
		ft_update_envlst(field, ft_join(old_content, content_args, ENV),
			&ft_shell()->env_lst, false);
	ft_update_envlst(field, content_args, &ft_shell()->expanded_env, true);
}

void	update_both_env_lst(char *field, char *content_args)
{
	char	*old_content;

	old_content = get_env_content(field, &ft_shell()->env_lst);
	ft_update_envlst(field, ft_join(old_content, content_args, ENV),
		&ft_shell()->env_lst, false);
	ft_update_envlst(field, ft_join(old_content, content_args, ENV),
		&ft_shell()->expanded_env, false);
}

int	check_export_concat(char *field, char **args)
{
	int		field_length;
	char	*content_args;

	field_length = ft_strlen(field);
	if (field[field_length - 1] == '+' && (ft_isalnum(field[field_length - 2])
			|| field[field_length - 2] == '_'))
	{
		content_args = ft_extract_content(args[1]);
		field[field_length - 1] = '\0';
		if (!ft_env_entry_exists(field, &ft_shell()->env_lst))
			create_update_both_env_lst(field, content_args);
		else if (ft_env_entry_exists(field, &ft_shell()->env_lst)
			&& !ft_env_entry_exists(field, &ft_shell()->expanded_env))
			create_one_update_one_env_lst(field, content_args);
		else
			update_both_env_lst(field, content_args);
		return (1);
	}
	return (0);
}
