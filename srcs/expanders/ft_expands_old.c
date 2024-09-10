/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expands_old.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 21:34:34 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/09 23:14:11 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_handle_dollar(char *str, size_t *i)
{
	size_t	start;
	char	*var;
	char	*env_val;
	
	(*i)++;
	if (ft_isdigit(str[*i]) || str[*i] == '@')
		return ((*i)++, ft_strdup(""));
	else if (str[*i] == '?')
	{
		(*i)++;
		return (ft_itoa((ft_shell())->exit_status));
	}
	else if (!ft_is_valid_var_char(str[*i]))
		return (ft_strdup("$"));
	start = *i;
	while (ft_is_valid_var_char(str[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	// printf("THE VAR IS: %s\n", var);
	env_val = ft_get_envlst_content(var, (ft_shell()));
	if (!env_val)
		return (free(var), ft_strdup(str));
	return (free(var), ft_strdup(env_val));
}

char	*ft_check_squotes(char *sr, size_t *x)
{
	size_t	start;

	start = *x;
	(*x)++;
	while (sr[*x] != '\'')
		(*x)++;
	(*x)++;
	return (ft_substr(sr, start, *x - start));
}

static char	*ft_handle_dquote_str(char *str, size_t *i)
{
	size_t	start;

	start = *i;
	while (str[*i] != '"' && str[*i] != '$')
	{
		(*i)++;
	}
	return (ft_substr(str, start, *i - start));
}

char	*ft_check_dquotes(char *sr, size_t *k)
{
	char	*res;

	res = ft_strdup("\"");
	(*k)++;
	while (sr[*k] && sr[*k] != '"')
	{
		if (sr[*k] == '$')
			res = ft_strjoin(res, ft_handle_dollar(sr, k));
		else
			res = ft_strjoin(res, ft_handle_dquote_str(sr, k));
	}
	(*k)++;
	return (ft_strjoin(res, ft_strdup("\"")));
}

char	*ft_handle_normal_str(char *sr, size_t *y)
{
	int	start;

	start = *y;
	(*y)++;
	while (sr[*y] && sr[*y] != '\'' && sr[*y] != '"' && sr[*y] != '$')
		(*y)++;
	return (ft_substr(sr, start, *y - start));
}

char	*ft_pre_expand(char *sr)
{
	size_t	x;
	char	*res;
	char	*temp;

	x = 0;
	res = msh_strdup("", PARSING);;
	while (sr[x] && sr[x] != '\0')
	{
		if (sr[x] == '"')
			res = ft_strjoin(res, ft_check_dquotes(sr, &x));
		else if (sr[x] == '\'')
			res = ft_strjoin(res, ft_check_squotes(sr, &x));
		else if (sr[x] == '$')
			res = ft_strjoin(res, ft_handle_dollar(sr, &x));
		else
		{	
			temp = ft_handle_normal_str(sr, &x);
			res = ft_strjoin(res, temp);
			free(temp);
		}
	}
	return (res);
}

size_t	get_arr_len(char **expanded)
{
	size_t	x;

	x = 0;
	if (!expanded || !expanded[0])
		return (0);
	while (expanded && expanded[x])
		x++;
	// printf("THE LENS IS: %zu\n", x);
	return (x);
}

char	**ft_expand(char *sr)
{
	char	**expanded;
	char	**globaled;
	size_t	x;

	x = 0;
	sr = ft_pre_expand(sr);
	if (!sr)
		return (NULL);
	sr = ft_clean_empty_chars(sr);
	if (!sr)
		return (NULL);
	globaled = ft_globaler(sr);
	free(sr);
	expanded = malloc(sizeof(char *) * (get_arr_len(globaled) + 1));
	if (!expanded)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), NULL);
	while (globaled[x])
	{
		expanded[x] = ft_strip_quotes(globaled[x]);
        expanded[x] = msh_strtrim_spaces(expanded[x]);
		x++;
	}
	free(globaled);
	expanded[x] = 0;
	return (expanded);
}
