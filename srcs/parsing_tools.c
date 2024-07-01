/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:53:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/28 18:12:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_an_operator(char c)
{
	return (c == '|' || c == '&' || c == '(' || c == ')' || c == '<'
		|| c == '>');
}

char	*find_operator(char *str)
{
	int	i;

	i = -1;
	if (!str | !str[0])
		return (NULL);
	while (str[++i])
	{
		if (is_an_operator(str[i]))
			return (str + i);
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
