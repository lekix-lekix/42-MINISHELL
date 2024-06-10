/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tools.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:53:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/10 11:54:11 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_strlen_sep(char *str, char *sep)
{
	int	i;

	i = 0;
	if (!sep)
		return (ft_strlen(str));
	while (str[i] && str[i] != *sep)
		i++;
	return (i + 1);
}

int	is_an_operator(char c)
{
	return (c == '|' || c == '&' || c == '(' || c == ')' || c == '<' || c == '>');
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

int	ft_strcpy_sep(char *dest, char *input, char *sep)
{
	int	i;

	i = -1;
	if (!sep)
		return (ft_strlcpy(dest, input, ft_strlen(input) + 1));
	while (input[++i] && input[i] != sep[0])
		dest[i] = input[i];
	dest[i] = '\0';
	return (i);
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
