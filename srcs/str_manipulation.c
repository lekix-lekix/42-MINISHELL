/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_manipulation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:04:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/03 13:25:53 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

size_t ft_strlen_loco (const char *s)
{
	size_t x;

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
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(final_str, s, s_len + 1);
	return (final_str);
}
