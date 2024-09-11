/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expands.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 21:34:34 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 22:28:46 by kipouliq         ###   ########.fr       */
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
			return (gbg_coll(res, PARSING, ADD), ft_close_fds(), exit(255),
				NULL);
		return ((*i)++, res);
	}
	else if (str[*i] == '?')
	{
		(*i)++;
		res = ft_itoa(ft_shell()->exit_status);
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
				NULL);
		return (res);
	}
	else if (!ft_is_valid_var_char(str[*i]))
	{
		res = msh_strdup("", PARSING);
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(res, PARSING, ADD), ft_close_fds(), exit(255),
				NULL);
		return (res);
	}
	start = *i;
	while (ft_is_valid_var_char(str[*i]))
		(*i)++;
	var = ft_substr(str, start, *i - start);
	// printf("THE FAR IS HERE: %s\n", var);
	if (!var)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	env_val = ft_get_envlst_content(var, &ft_shell()->env_lst);
	if (!env_val)
	{
		// printf("WE DIDN'T find env val\n");
		res = msh_strdup("", PARSING);
		return (free(var), res);
	}
	// dprintf(2, "var = %s\n", var);
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
    // dprintf(2, "res substr sq = %s x = %zu\n", res, *x);
	if (!res || gbg_coll(res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	return (res);
}

static char	*ft_handle_dquote_str(char *str, size_t *i)
{
	size_t	start;
	char	*res;

	start = *i;
	while (str[*i] && str[*i] != '"' && (str[*i] != '$' || ft_is_space(str[*i
				+ 1]) || str[*i + 1] == '"'))
		(*i)++;
	res = ft_substr(str, start, *i - start);
    // dprintf(2, "res substr dq = %s\n", res);
	if (!res || gbg_coll(res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	return (res);
}

char	*ft_check_dquotes(char *sr, size_t *k)
{
	char	*res;
	char	*final_res;

    // dprintf(2, "dquotes\n");
	res = msh_strdup("\"", PARSING);
	(*k)++;
	while (sr[*k] && sr[*k] != '"')
	{
		if (sr[*k] == '$' && (!ft_is_space(sr[*k + 1]) && sr[*k + 1] != '"'))
			res = ft_strjoin(res, ft_handle_dollar(sr, k));
		else
			res = ft_strjoin(res, ft_handle_dquote_str(sr, k));
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
				NULL);
	}
	(*k)++;
	final_res = msh_strdup("\"", PARSING);
	final_res = ft_strjoin(res, final_res);
	if (!final_res || gbg_coll(final_res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
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
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
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
        // printf("x = %zu, sr = %s\n", x, sr);
		if (sr[x] == '"')
			res = ft_strjoin(res, ft_check_dquotes(sr, &x));
		else if (sr[x] == '\'')
			res = ft_strjoin(res, ft_check_squotes(sr, &x));
		else if (sr[x] == '$' && (!ft_is_space(sr[x + 1]) && sr[x + 1] != '\0'))
			res = ft_strjoin(res, ft_handle_dollar(sr, &x));
		else
		{
			temp = ft_handle_normal_str(sr, &x);
			if (!temp)
				return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(),
					exit(255), NULL);
			res = ft_strjoin(res, temp);
		}
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
				NULL);
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
	return (x);
}

char	**ft_expand(char *sr)
{
	char	**expanded;
	char	**globaled;
	char	*tmp;
	size_t	x;

	x = 0;
	// dprintf(2, "THE Incoming arg: %s\n", sr);
	tmp = ft_pre_expand(sr);
    // dprintf(2, "tmp = %s\n", tmp);
	if (!tmp || !tmp[0])
		return (NULL);
	// if (ft_shell()->expand_chars_trimmed != 0)
	// 	tmp = ft_join(tmp, sr + ft_shell()->expand_chars_trimmed + 1);
	tmp = ft_clean_empty_chars(tmp);
	// printf("After cleaning arg: %s\n", tmp);
	globaled = ft_globaler(tmp);
	expanded = malloc(sizeof(char *) * (get_arr_len(globaled) + 1));
	if (!expanded || gbg_coll(expanded, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	while (globaled[x])
	{
		// printf("THE G: %s\n", globaled[x]);
		expanded[x] = ft_strip_quotes(globaled[x]);
		// expanded[x] = msh_strtrim_spaces(expanded[x]);
		x++;
	}
	expanded[x] = 0;
	return (expanded);
}

void	expand_redirections(t_redir **redirections)
{
	t_redir	*current;
	// char	**tmp;

	current = *redirections;
	while (current)
	{
		current->filename = ft_pre_expand(current->filename);
		current->filename = ft_strip_quotes(current->filename);
		// if (current->filename && ft_is_contains_asterisk(current->filename))
		// {
		// 	tmp = ft_globaler(current->filename);
		// 	current->filename = tmp[0];
		// }
		current = current->next;
	}
}
