/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expands_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/11 13:15:23 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/05 06:03:04 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

bool ft_is_valid_var_char (char c)
{
    if (ft_isalnum(c) || c == '_')
        return (true);
    return (false);
}


bool ft_is_contains_asterisk (char *sr)
{
    size_t x;

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