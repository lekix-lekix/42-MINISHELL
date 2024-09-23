/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_strtrim_spaces.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:33:34 by lekix             #+#    #+#             */
/*   Updated: 2024/09/11 20:37:29 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int	ft_total_to_trim(char const *str)
{
	int	i;
	int	count;

	i = -1;
	count = 0;
	if (!str)
		return (0);
	while (ft_is_space(str[++i]))
		count++;
	i = ft_strlen(str);
	while (ft_is_space(str[--i]))
		count++;
	return (count);
}

void	trim_spaces(char const *s1, char *final_str)
{
	int	i;
	int	j;
	int	k;

	i = 0;
	j = 0;
	while (s1[i] && ft_is_space(s1[i]))
		i++;
	k = ft_strlen(s1) - 1;
	while (s1[k] && ft_is_space(s1[k]))
		k--;
	while (i <= k)
	{
		final_str[j] = s1[i];
		i++;
		j++;
	}
	final_str[j] = '\0';
}

char	*msh_strtrim_spaces(char const *s1)
{
	int		str_size;
	char	*final_str;

	if (!s1)
		return (NULL);
	str_size = ft_strlen(s1) - ft_total_to_trim(s1) + 1;
	if (str_size < 0)
		return (empty_str());
	final_str = malloc(sizeof(char) * str_size);
	if (!final_str || gbg_coll(final_str, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	trim_spaces(s1, final_str);
	return (final_str);
}
