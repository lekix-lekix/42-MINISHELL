/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_handle_money.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:52:19 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/20 13:48:05 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*dollar_first_digit(size_t *i)
{
	char	*res;

	res = msh_strdup("", PARSING);
	if (!res || gbg_coll(res, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	return ((*i)++, res);
}

char	*ft_dollar_explanation_mark(size_t *i)
{
	char	*res;

	(*i)++;
	res = ft_itoa(ft_shell()->exit_status);
	if (!res || gbg_coll(res, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	return (res);
}

char	*ft_dollar_valid(void)
{
	char	*res;

	res = msh_strdup("", PARSING);
	return (res);
}

char	*ft_handle_dollar(char *str, size_t *i)
{
	size_t	start;
	char	*var;
	char	*env_val;
	char	*res;

	(*i)++;
	if (ft_isdigit(str[*i]) || str[*i] == '@')
		return (dollar_first_digit(i));
	else if (str[*i] == '?')
		return (ft_dollar_explanation_mark(i));
	else if (!ft_is_valid_var_char(str[*i]))
		return (ft_dollar_valid());
	start = *i;
	while (ft_is_valid_var_char(str[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	if (!var)
		return (ft_exit_close(255), NULL);
	env_val = ft_get_envlst_content(var, &ft_shell()->env_lst);
	if (!env_val)
		return (free(var), msh_strdup("", PARSING));
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
