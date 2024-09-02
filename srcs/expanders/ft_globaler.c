/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_globaler.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:26:53 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/02 15:43:44 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static bool	ft_matches_visibility(char *pattern, char *str)
{
	if ((pattern[0] == '.' && str[0] == '.') || (pattern[0] != '.'
			&& str[0] != '.'))
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
		if (!returned || gbg_coll(returned, PARSING, ADD))
		    return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		returned[0] = msh_strdup(str, PARSING);
		returned[1] = 0;
		return (returned);
	}
	else
	{
		dir = opendir(".");
		// printf("THE DIR");
		returned = (char **)ft_calloc(match_count + 1, sizeof(char *));
		if (!returned || gbg_coll(returned, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		match_count = 0;
		while (ft_set_direntry(&entry, dir) && entry)
			if (ft_match_star(str, entry->d_name) && ft_matches_visibility(str,
					entry->d_name))
				returned[match_count++] = ft_strdup(entry->d_name);
		closedir(dir);
	}
	return (returned);
}
