/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:15:23 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 22:27:18 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool	ft_is_valid_var_char(char c)
{
	if (ft_isalnum(c) || c == '_')
		return (true);
	return (false);
}

bool	ft_is_contains_asterisk(char *sr)
{
	size_t	x;

	x = 0;
	while (sr[x])
	{
		if (sr[x] == '*')
			return (true);
		x++;
	}
	return (false);
}

size_t	ft_count_match(char *pattern)
{
	DIR				*dir;
	size_t			match_count;
	struct dirent	*entry;

	match_count = 0;
	dir = opendir(".");
	entry = readdir(dir);
	while (entry)
	{
		if (ft_match_star(pattern, entry->d_name))
			match_count++;
		entry = readdir(dir);
	}
	closedir(dir);
	return (match_count);
}

char	*ft_handle_dquote_str(char *str, size_t *i)
{
	size_t	start;
	char	*res;

	start = *i;
	while (str[*i] && str[*i] != '"' && (str[*i] != '$' || ft_is_space(str[*i
				+ 1]) || str[*i + 1] == '"'))
		(*i)++;
	res = ft_substr(str, start, *i - start);
	if (!res || gbg_coll(res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	return (res);
}

size_t	get_arr_len(char **expanded)
{
	size_t x;

	x = 0;
	if (!expanded || !expanded[0])
		return (0);
	while (expanded && expanded[x])
		x++;
	return (x);
}
