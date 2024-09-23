/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_cmd.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 14:02:15 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/20 14:03:36 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	expand_cmd(t_token *cmd)
{
	char	**la_args;
	char	**tmp_contents;
	int		idx;
	int		words_nb;

	idx = -1;
	tmp_contents = NULL;
	while (cmd->type == CMD && cmd->contents[++idx])
	{
		la_args = ft_expand(cmd->contents[idx]);
		if (!la_args)
		{
			tmp_contents = dup_arr_join_empty_str(tmp_contents);
			continue ;
		}
		if (str_contains_expand(cmd->contents[idx]))
			words_nb = content_count_words(la_args[0]);
		else
			words_nb = content_count_words(cmd->contents[idx]);
		if (get_arr_len(la_args) == 1 && words_nb > 1)
			la_args = msh_split_spaces(la_args[0], PARSING);
		tmp_contents = ft_concat_str_arr_idx(tmp_contents, la_args);
	}
	cmd->contents = tmp_contents;
	return (0);
}
