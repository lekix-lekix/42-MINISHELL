/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   paring_word_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:55:04 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 19:55:34 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	get_end_quote(char *str, int i, char c)
{
	while (str && str[i] != c)
		i++;
	return (i);
}

int	content_count_words(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		while (str[i] && ft_is_space(str[i]))
			i++;
		if (str[i])
			count++;
		if (is_quote(str[i]))
			i = get_end_quote(str, i + 1, str[i]) + 1;
		while (str[i] && !ft_is_space(str[i]))
		{
			if (is_quote(str[i]))
				i = get_end_quote(str, i + 1, str[i]);
			i++;
		}
	}
	return (count);
}

int	get_end_word_idx(char *str, int i)
{
	while (str[i])
	{
		if (is_quote(str[i]))
			i = get_end_quote(str, i + 1, str[i]);
		if (ft_is_space(str[i]))
			return (i);
		i++;
	}
	return (i);
}
