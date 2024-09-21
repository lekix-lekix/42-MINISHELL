/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_manipulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:04:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/21 13:06:20 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t	ft_strlen_loco(const char *s)
{
	size_t	x;

	x = 0;
	if (!s || !s[0])
		return (0);
	while (s[x] && s[x] != '\0')
		x++;
	return (x);
}

char	*msh_strdup(const char *s, int mlc_lst)
{
	char	*final_str;
	int		s_len;

	s_len = ft_strlen_loco(s);
	final_str = malloc(sizeof(char) * (s_len + 1));
	if (!final_str || gbg_coll(final_str, mlc_lst, ADD))
		return (ft_exit_close(255), NULL);
	ft_strlcpy(final_str, s, s_len + 1);
	return (final_str);
}

char	**ft_concat_str_arr(char **arr, char **arr2)
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
	while (++x < len1)
		res[x] = msh_strdup(arr[x], PARSING);
	y = -1;
	while (++y < len2)
	{
		res[x] = msh_strdup(arr2[y], PARSING);
		x++;
	}
	res[x] = NULL;
	return (res);
}
