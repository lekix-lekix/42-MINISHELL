/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expands.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/10 21:34:34 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/20 13:47:54 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

char	*ft_check_dquotes(char *sr, size_t *k)
{
	char	*res;
	char	*final_res;

	res = msh_strdup("\"", PARSING);
	(*k)++;
	while (sr[*k] && sr[*k] != '"')
	{
		if (sr[*k] == '$' && (!ft_is_space(sr[*k + 1]) && sr[*k + 1] != '"'))
			res = ft_strjoin(res, ft_handle_dollar(sr, k));
		else
			res = ft_strjoin(res, ft_handle_dquote_str(sr, k));
		if (!res || gbg_coll(res, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_exit_close(255), NULL);
	}
	(*k)++;
	final_res = msh_strdup("\"", PARSING);
	final_res = ft_strjoin(res, final_res);
	if (!final_res || gbg_coll(final_res, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_exit_close(255), NULL);
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
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_exit_close(255), NULL);
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
		else if (sr[x] == '$' && (!ft_is_space(sr[x + 1]) && sr[x + 1] != '\0'))
			res = ft_strjoin(res, ft_handle_dollar(sr, &x));
		else
		{
			temp = ft_handle_normal_str(sr, &x);
			if (!temp)
				return (ft_exit_close(255), NULL);
			res = ft_strjoin(res, temp);
		}
		if (!res || gbg_coll(res, PARSING, ADD))
			return (ft_exit_close(255), NULL);
	}
	return (res);
}

char	**ft_expand(char *sr)
{
	char	**expanded;
	char	**globaled;
	char	*tmp;
	size_t	x;

	x = 0;
	tmp = ft_pre_expand(sr);
	if (!tmp || !tmp[0])
		return (NULL);
	tmp = ft_clean_empty_chars(tmp);
	globaled = ft_globaler(tmp);
	expanded = malloc(sizeof(char *) * (get_arr_len(globaled) + 1));
	if (!expanded || gbg_coll(expanded, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	while (globaled[x])
	{
		expanded[x] = ft_strip_quotes(globaled[x]);
		x++;
	}
	expanded[x] = 0;
	return (expanded);
}

void	expand_redirections(t_redir **redirections)
{
	t_redir	*current;

	current = *redirections;
	while (current)
	{
		current->filename = ft_pre_expand(current->filename);
		current->filename = ft_strip_quotes(current->filename);
		current = current->next;
	}
}
