/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_remove_quotes.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 14:36:19 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/02 14:34:50 by sabakar-         ###   ########.fr       */
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
    // printf("THE i LENGTH: %zu\n", *i);
    // printf("THE J LENGTH: %zu\n", *j);
    // printf("----------------------\n");
    // printf("THE i str: %c\n", str[*i]);
    // printf("THE j str: %c\n", ret[*j]);
    // printf("----------------------\n");
    // printf("THE i str: %s\n", str);
    // printf("THE j str: %s\n", ret);
    // printf("----------------------\n");
    // printf("THE STE: %c\n", quotes);
    if (!ft_strncmp_loco(str, "", ft_strlen("")))
    {	
        *ret = *msh_strdup(" ", PARSING);
    }
    while (str[*i] != quotes)
    {
        ret[(*j)++] = str[(*i)++];
		// printf("THE RET CHAR: %s\n", ret);
    }
    // printf("TEH STE: %s\n", ret);
    //    if (str[*i] == quotes)
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
        return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
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

char	*ft_clean_empty_chars(char *sr)
{
	size_t	x;
	size_t	y;
	char	*tmp;
	char	*ret;
	size_t	dstsize;

	if ((sr[0] == '\'' && sr[1] == '\'' && !sr[2]) || (sr[0] == '"'
			&& sr[1] == '"' && !sr[2]))
		return (sr);
	tmp = ft_calloc(ft_strlen(sr) + 1, sizeof(char));
	if (!tmp || gbg_coll(tmp, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	x = 0;
	y = 0;
	while (sr[x])
	{
		if ((sr[x] == '\'' && sr[x + 1] == '\'') || (sr[x] == '"' && sr[x
				+ 1] == '"'))
			x += 2;
		else
			tmp[y++] = sr[x++];
	}
	dstsize = ft_strlen(tmp) + 1;
	ret = ft_calloc(dstsize, sizeof(char));
	if (!ret || gbg_coll(ret, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (ft_strlcpy(ret, tmp, dstsize), ret);
}
