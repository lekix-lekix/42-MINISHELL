/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 07:14:54 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/03 12:52:56 by sabakar-         ###   ########.fr       */
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
		return ;
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
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	i = -1;
	while (s1[++i])
		new_arr[i] = s1[i];
	i = -1;
	while (s2[++i])
		new_arr[s_len + i] = s2[i];
	new_arr[s_len + i] = '\0';
	return (new_arr);
}

bool	ft_env_entry_exists(char *content)
{
	t_env	*envlst;

	envlst = (ft_shell())->expanded_env;
	while (envlst)
	{
		if (!ft_strcmp(content, envlst->content))
			return (true);
		envlst = envlst->next;
	}
	return (false);
}
