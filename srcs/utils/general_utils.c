/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 07:14:54 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/21 12:14:39 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_print_err(char *str)
{
	int		x;
	int		len;
	char	*new_str;

	len = ft_strlen(str);
	x = 0;
	new_str = (char *)malloc(sizeof(char) * len + 1);
	if (!new_str)
		ft_exit_close(255);
	while (str[x])
	{
		new_str[x] = str[x];
		x++;
	}
	new_str[x] = '\n';
	write(2, new_str, len + 1);
	free(new_str);
}

void	ft_free(char **arr)
{
	int	x;

	x = 0;
	if (!arr)
		return ;
	while (arr[x])
	{
		free(arr[x]);
		arr[x] = NULL;
		x++;
	}
	free(arr);
	arr = NULL;
}

char	*ft_join(char *s1, char *s2)
{
	char	*new_arr;
	int		i;
	int		s_len;
	int		total_len;

	if (!s1 || !s2)
		return (NULL);
	s_len = ft_strlen(s1);
	total_len = (ft_strlen(s1) + ft_strlen(s2));
	new_arr = (char *)malloc(sizeof(char) * (total_len + 1));
	if (!new_arr || gbg_coll(new_arr, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	i = -1;
	while (s1[++i])
		new_arr[i] = s1[i];
	i = -1;
	while (s2[++i])
		new_arr[s_len + i] = s2[i];
	new_arr[s_len + i] = '\0';
	return (new_arr);
}

bool	ft_env_entry_exists(char *field, t_env **lst_to_check)
{
	t_env	*envlst;

	envlst = *lst_to_check;
	while (envlst)
	{
		if (!ft_strcmp(field, envlst->field))
			return (true);
		envlst = envlst->next;
	}
	return (false);
}

void	ft_close_fds(void)
{
	close(ft_shell()->ft_stdin);
	close(ft_shell()->ft_stdout);
}
