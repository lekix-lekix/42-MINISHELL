/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expands.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 21:34:34 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/22 17:03:16 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_handle_dollar(char *str, size_t *i)
{
	size_t	start;
	char	*var;
	char	*env_val;
	char	*res;

	(*i)++;
	if (ft_isdigit(str[*i]) || str[*i] == '@')
	{
		res = ft_strdup("");
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(res, PARSING, ADD), exit(255), NULL);
		return ((*i)++, res);
	}
	else if (str[*i] == '?')
	{
		(*i)++;
		res = ft_itoa(ft_shell()->exit_status);
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		return (res);
	}
	else if (!ft_is_valid_var_char(str[*i]))
	{
		res = ft_strdup("$");
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(res, PARSING, ADD), exit(255), NULL);
		return (res);
	}
	start = *i;
	while (ft_is_valid_var_char(str[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	if (!var)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	env_val = ft_get_envlst_content(var, ft_shell());
	if (!env_val)
	{
		res = ft_strdup(str);
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		return (free(var), res);
	}
	res = msh_strdup(env_val, PARSING);
	return (free(var), res);
}

char	*ft_check_squotes(char *sr, size_t *x)
{
	size_t	start;
	char	*res;

	start = *x;
	(*x)++;
	while (sr[*x] != '\'')
		(*x)++;
	(*x)++;
	res = ft_substr(sr, start, *x - start);
	if (!res || gbg_coll(res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (res);
}

static char	*ft_handle_dquote_str(char *str, size_t *i)
{
	size_t	start;
	char	*res;

	start = *i;
	while (str[*i] != '"' && str[*i] != '$')
	{
		(*i)++;
	}
	res = ft_substr(str, start, *i - start);
	if (!res || gbg_coll(res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (res);
}

char	*ft_check_dquotes(char *sr, size_t *k)
{
	char	*res;
	char	*final_res;

	res = msh_strdup("\"", PARSING);
	(*k)++;
	while (sr[*k] && sr[*k] != '"')
	{
		if (sr[*k] == '$')
			res = ft_strjoin(res, ft_handle_dollar(sr, k));
		else
			res = ft_strjoin(res, ft_handle_dquote_str(sr, k));
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	}
	(*k)++;
	final_res = msh_strdup("\"", PARSING);
	final_res = ft_strjoin(res, final_res);
	if (!final_res || gbg_coll(final_res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (final_res);
}

char	*ft_handle_normal_str(char *sr, size_t *y)
{
	int		start;
	char	*final_res;

	start = *y;
	(*y)++;
	while (sr[*y] && sr[*y] != '\'' && sr[*y] != '"' && sr[*y] != '$')
		(*y)++;
	final_res = ft_substr(sr, start, *y - start);
	if (!final_res || gbg_coll(final_res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (final_res);
}

char	*ft_pre_expand(char *sr)
{
	size_t	x;
	char	*res;
	char	*temp;

	x = 0;
	res = msh_strdup("", PARSING);
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
			if (!temp)
				return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
			res = ft_strjoin(res, temp);
			// free(temp);
		}
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
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
	// if (!sr)x
	// 	return (NULL);
	globaled = ft_globaler(sr);
	// free(sr);
	expanded = malloc(sizeof(char *) * (get_arr_len(globaled) + 1));
	if (!expanded || gbg_coll(expanded, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), NULL);
	while (globaled[x])
	{
		expanded[x] = ft_strip_quotes(globaled[x]);
		x++;
	}
	// free(globaled);
	expanded[x] = 0;
	return (expanded);
}
