/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msh_strtrim_spaces.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 17:33:34 by lekix             #+#    #+#             */
/*   Updated: 2024/09/03 18:43:02 by kipouliq         ###   ########.fr       */
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

static char	*empty_str(void)
{
	char	*empty_str;

	empty_str = malloc(1);
	if (!empty_str || gbg_coll(empty_str, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	empty_str[0] = '\0';
	return (empty_str);
}

char	*msh_strtrim_spaces(char const *s1)
{
	int		i;
    int     j;
	int		str_size;
	char	*final_str;

	if (!s1)
		return (NULL);
	str_size = ft_strlen(s1) - ft_total_to_trim(s1) + 1;
	if (str_size < 0)
		return (empty_str());
	final_str = malloc(sizeof(char) * str_size);
	if (!final_str || gbg_coll(final_str, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	i = 0;
    j = 0;
    while (s1[i] && ft_is_space(s1[i]))
        i++;
    while (s1[i] && !ft_is_space(s1[i]))
    {
        final_str[j] = s1[i];
        i++;
        j++;
    }
	final_str[j] = '\0';
	return (final_str);
}
