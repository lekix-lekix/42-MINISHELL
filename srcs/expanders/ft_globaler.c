/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_globaler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:26:53 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/05 06:05:31 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	ft_matches_visibility(char *pattern, char *str)
{
	if ((pattern[0] == '.' && str[0] == '.')
		|| (pattern[0] != '.' && str[0] != '.'))
		return (true);
	return (false);
}

static bool	ft_set_direntry(struct dirent **entry, DIR *dir)
{
	*entry = readdir(dir);
	return (true);
}

char	**ft_globaler(char *str)
{
	char			**returned;
	DIR				*dir;
	struct dirent	*entry;
	size_t			match_count;

	match_count = ft_count_match(str);
	// printf("THE COUNT MATCH NUM %zu\n", match_count);
	if (!ft_is_contains_asterisk(str) || !match_count)
	{
		// returned = (char **)ft_calloc(2, sizeof(char *));
		returned = malloc(sizeof(char *) * 2);
		returned[0] = msh_strdup(str, PARSING);
		returned[1] = NULL;
		return (returned);
	}
	else
	{
		dir = opendir(".");
		// printf("THE DIR");
		returned = (char **)ft_calloc(match_count + 1, sizeof(char *));
		match_count = 0;
		while (ft_set_direntry(&entry, dir) && entry)
			if (ft_match_star(str, entry->d_name)
				&& ft_matches_visibility(str, entry->d_name))
				returned[match_count++] = ft_strdup(entry->d_name);
		closedir(dir);
	}
	return (returned);
}
