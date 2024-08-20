/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_strtrim.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:33:34 by lekix             #+#    #+#             */
/*   Updated: 2024/08/20 16:16:29 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_is_in_set(char c, char const *set)
{
	int	i;

	i = -1;
	while (set[++i])
	{
		if (set[i] == c)
			return (1);
	}
	return (0);
}

static int	ft_total_to_trim(char const *str, char const *set, int *begin)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	if (!str)
		return (0);
	while (ft_is_in_set(str[++i], set))
		count++;
	*begin = i;
	i = ft_strlen(str);
	while (ft_is_in_set(str[--i], set))
		count++;
	return (count);
}

char	*empty_str(void)
{
	char	*empty_str;

	empty_str = malloc(1);
	if (!empty_str || gbg_coll(empty_str, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	empty_str[0] = '\0';
	return (empty_str);
}

char	*msh_strtrim(char const *s1, char const *set)
{
	int		i;
	int		j;
	int		str_size;
	char	*final_str;

	if (!s1 || !set)
		return (NULL);
	str_size = ft_strlen(s1) - ft_total_to_trim(s1, set, &i) + 1;
	if (str_size < 0)
		return (empty_str());
	final_str = malloc(sizeof(char) * str_size);
	if (!final_str || gbg_coll(final_str, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	j = 0;
	while (s1[i] && j < str_size - 1)
	{
		final_str[j] = s1[i++];
		j++;
	}
	final_str[j] = '\0';
	return (final_str);
}
