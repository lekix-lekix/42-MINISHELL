/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_money.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:52:19 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 20:18:39 by sabakar-         ###   ########.fr       */
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
		res = msh_strdup("", PARSING);
		if (!res || gbg_coll(res, PARSING, ADD))
			return (ft_exit_close(255), NULL);
		return ((*i)++, res);
	}
	else if (str[*i] == '?')
	{
		(*i)++;
		res = ft_itoa(ft_shell()->exit_status);
		if (!res || gbg_coll(res, PARSING, ADD))
			return (ft_exit_close(255), NULL);
		return (res);
	}
	else if (!ft_is_valid_var_char(str[*i]))
	{
		res = msh_strdup("", PARSING);
		if (!res || gbg_coll(res, PARSING, ADD))
			return (ft_exit_close(255), NULL);
		return (res);
	}
	start = *i;
	while (ft_is_valid_var_char(str[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	if (!var)
		return (ft_exit_close(255), NULL);
	env_val = ft_get_envlst_content(var, &ft_shell()->env_lst);
	if (!env_val)
	{
		res = msh_strdup("", PARSING);
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
	while (sr[*x] && sr[*x] != '\'')
		(*x)++;
	(*x)++;
	res = ft_substr(sr, start, *x - start);
	if (!res || gbg_coll(res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	return (res);
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
	size_t	x;

	x = 0;
	if (!expanded || !expanded[0])
		return (0);
	while (expanded && expanded[x])
		x++;
	return (x);
}