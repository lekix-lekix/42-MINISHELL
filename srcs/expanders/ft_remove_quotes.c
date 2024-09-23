/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:36:19 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 23:04:23 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static size_t	ft_unquoted_strlen(char *str)
{
	size_t	i;
	size_t	len;
	char	quotes;

	i = 0;
	len = 0;
	quotes = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			if (!quotes)
				quotes = str[i++];
			else if (str[i] == quotes)
				quotes = ((i++) && 0);
			else
				len += (i++ || 1);
		}
		else
			len += (i++ || 1);
	}
	return (len);
}

static void	ft_unquote_filler(char *str, size_t *i, char *ret, size_t *j)
{
	char	quotes;

	quotes = str[(*i)++];
	while (str[*i] != quotes)
		ret[(*j)++] = str[(*i)++];
	(*i)++;
}

char	*ft_strip_quotes(char *str)
{
	char	*ret;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	ret = ft_calloc(1 + ft_unquoted_strlen(str), sizeof(char));
	if (!ret || gbg_coll(ret, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
			ft_unquote_filler(str, &i, ret, &j);
		else
			ret[j++] = str[i++];
	}
	ret[j] = '\0';
	return (ret);
}

int	parse_find_quotes(char *sr, char *tmp, int *is_diff)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (sr[x])
	{
		if (((sr[x] == '\'' && sr[x + 1] == '\'') || (sr[x] == '"' && sr[x
						+ 1] == '"')) && *is_diff)
			x += 2;
		else
		{
			*is_diff = 0;
			tmp[y++] = sr[x++];
		}
	}
	return (0);
}

char	*ft_clean_empty_chars(char *sr)
{
	char	*tmp;
	char	*ret;
	size_t	dstsize;
	int		is_diff;

	if (!sr || !sr[0])
		return (sr);
	is_diff = 23;
	if ((sr[0] == '\'' && sr[1] == '\'' && !sr[2]) || (sr[0] == '"'
			&& sr[1] == '"' && !sr[2]))
		return (sr);
	tmp = ft_calloc(ft_strlen(sr) + 1, sizeof(char));
	if (!tmp || gbg_coll(tmp, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	parse_find_quotes(sr, tmp, &is_diff);
	dstsize = ft_strlen(tmp) + 1;
	ret = ft_calloc(dstsize, sizeof(char));
	if (!ret || gbg_coll(ret, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	return (ft_strlcpy(ret, tmp, dstsize), ret);
}
