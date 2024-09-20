/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 11:55:40 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/20 12:14:51 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	trim_contents(t_token **lst)
{
	t_token	*current;

	current = *lst;
	while (current)
	{
		if (current->content)
			current->content = msh_strtrim_spaces(current->content);
		current = current->next;
	}
}

int	str_contains_spaces(char *str)
{
	int	i;

	i = -1;
	while (str[++i])
	{
		if (ft_is_space(str[i]))
			return (1);
	}
	return (0);
}

int	str_contains_expand(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$' || str[i] == '*')
			return (1);
		i++;
	}
	return (0);
}

char	**ft_concat_str_arr_idx(char **arr, char **arr2)
{
	int		len1;
	int		len2;
	char	**res;
	int		x;
	int		y;

	len1 = get_arr_len(arr);
	len2 = get_arr_len(arr2);
	res = (char **)malloc(sizeof(char *) * (len1 + len2 + 2));
	if (!res || gbg_coll(res, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	x = -1;
	y = 0;
	while (arr && arr[++x])
		res[y++] = msh_strdup(arr[x], PARSING);
	x = -1;
	while (arr2 && arr2[++x])
	{
		res[y] = msh_strdup(arr2[x], PARSING);
		y++;
	}
	res[y] = NULL;
	return (res);
}

char	**dup_arr_join_empty_str(char **arr)
{
	char	**new_arr;
	int		i;

	new_arr = malloc(sizeof(char *) * (get_arr_len(arr) + 2));
	if (!new_arr || gbg_coll(new_arr, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	i = 0;
	while (arr && arr[i])
	{
		new_arr[i] = msh_strdup(arr[i], PARSING);
		i++;
	}
	new_arr[i] = empty_str();
	new_arr[i + 1] = NULL;
	i = 0;
	return (new_arr);
}
