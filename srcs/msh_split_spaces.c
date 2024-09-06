/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_split_spaces.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 14:00:08 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/04 16:10:14 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_count_words_spaces(char const *s)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] && ft_is_space(s[i]))
			i++;
		if (s[i] && !ft_is_space(s[i]))
			count++;
		while (s[i] && !ft_is_space(s[i]))
			i++;
	}
	return (count);
}

static char	*ft_malloc_cpy_spaces(char const *src, int n, int mlc_list)
{
	char	*final_str;
	int		i;

	i = -1;
	final_str = malloc(sizeof(char) * (n + 1));
	if (!final_str || gbg_coll(final_str, mlc_list, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255), NULL);
	while (src[++i] && i < n)
		final_str[i] = src[i];
	final_str[i] = '\0';
	return (final_str);
}

static void	ft_skip_spaces(char const *str, int *i, int skip_type)
{
	while (skip_type && ft_is_space(str[*i]) && str[*i])
		*i += 1;
	if (skip_type)
		return ;
	while (!skip_type && !ft_is_space(str[*i]) && str[*i])
		*i += 1;
	if (!skip_type)
		return ;
}

static int	ft_fill_arr(char **strs, char const *s, int mlc_list)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	k = -1;
	while (s[i])
	{
		i = 0;
		ft_skip_spaces(s, &i, 1);
		if (!s[i])
			return (0);
		j = i;
		ft_skip_spaces(s, &j, 0);
		if (!s[j] || ft_is_space(s[j]))
		{
			strs[++k] = ft_malloc_cpy_spaces(s + i, j - i, mlc_list);
			if (!strs[k])
				return (-1);
			if (!s[j])
				return (0);
			s += j;
		}
	}
	return (0);
}

char	**msh_split_spaces(char const *s, int mlc_list)
{
	char	**str_arr;
	int		strs_count;
	int		i;

	i = -1;
	if (!s)
		return (NULL);
	strs_count = ft_count_words_spaces(s);
	if (!strs_count || strs_count == 1)
		return (NULL);
	str_arr = malloc(sizeof(char *) * (strs_count + 1));
	if (!str_arr || gbg_coll(str_arr, mlc_list, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	while (++i <= strs_count)
		str_arr[i] = NULL;
	if (ft_fill_arr(str_arr, s, mlc_list) == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (str_arr);
}
