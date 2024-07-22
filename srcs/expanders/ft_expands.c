/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expands.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 21:34:34 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/22 15:21:37 by sabakar-         ###   ########.fr       */
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
	env_val = ft_get_envlst_content(var, (ft_shell()));
	if (!env_val)
		return (free(var), ft_strdup(""));
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

	x = 0;
	res = ft_strdup("");
	while (sr[x] && sr[x] != '\0')
	{
		if (sr[x] == '"')
			res = ft_strjoin(res, ft_check_dquotes(sr, &x));
		else if (sr[x] == '\'')
			res = ft_strjoin(res, ft_check_squotes(sr, &x));
		else if (sr[x] == '$')
			res = ft_strjoin(res, ft_handle_dollar(sr, &x));
		else
			res = ft_strjoin(res, ft_handle_normal_str(sr, &x));
	}
	return (res);
}

char	*ft_expand(char *sr)
{
	// char **expanded;
	char *pre_expand;

	sr = ft_pre_expand(sr);
	if (!sr)
		return (NULL);
	pre_expand = ft_strip_quotes(sr);
	printf("THE RETURNED STR: %s\n", pre_expand);
	// expanded = ft_expander_split(sr);
	// if (!expanded || expanded[0] == NULL)
	// 	return (NULL);
	// free(sr);
	// int x = -1;
	// while (expanded[++x])
	// 	printf("THE EXPANDED SH!T: %s\n", expanded[x]);
	// printf("Thee str pre expanding is: %s\n", pre_expand);
	return (pre_expand);
}