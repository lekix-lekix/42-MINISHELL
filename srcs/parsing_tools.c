/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:53:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/02 14:49:13 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_an_operator(char c)
{
	return (c == '|' || c == '&' || c == '(' || c == ')' || c == '<'
		|| c == '>');
}

int	is_a_quote(char c)
{
	return (c == '\"' || c == '\'');
}

int	find_next_quote(char *str, char quote, int i)
{
	while (str[++i])
	{
		if (str[i] == quote)
			return (i);
	}
	return (-1);
}

char	*find_operator(char *str)
{
	int	i;

	i = 0;
	if (!str | !str[0])
		return (NULL);
	while (str && str[i])
	{
		if (is_a_quote(str[i]))
			i = find_next_quote(str, str[i], i);
		if (is_an_operator(str[i]))
			return (str + i);
		i++;
	}
	return (NULL);
}

int	ft_is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

char	*skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_is_space(str[i]))
		i++;
	return (str + i);
}
