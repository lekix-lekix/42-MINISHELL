/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tools_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:05:08 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/26 17:03:58 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	only_spaces(char *str)
{
	int	i;

	i = 0;
	while (ft_is_space(str[i]))
		i++;
	if (!str[i])
		return (1);
	return (0);
}

int	check_op_len(char *str, int *op_len)
{
	int	i;

	i = 0;
	while (str[i] == str[0])
		i++;
	if (i > 2)
		return (0);
	*op_len = i;
	return (1);
}

int	is_a_token_operator(t_token *node)
{
	return (node->type > ARGS_FLAGS && node->type < PAR_LEFT);
}

int	ft_strcpy_sep_ptr(char *dest, char *input, char *sep)
{
	int	i;

	i = 0;
	if (!input)
	{
		dest[0] = '\0';
		return (1);
	}
	if (!sep)
		return (ft_strlcpy(dest, input, ft_strlen(input) + 1));
	while (input + i && input + i != sep)
    {
        dest[i] = input[i];
        i++;
    }
	dest[i] = '\0';
	return (i);
}

int	ft_strcpy_sep(char *dest, char *input, char *sep)
{
	int	i;

	i = -1;
	if (!input)
	{
		dest[0] = '\0';
		return (1);
	}
	if (!sep)
		return (ft_strlcpy(dest, input, ft_strlen(input) + 1));
	while (input[++i] && input[i] != sep[0])
		dest[i] = input[i];
	dest[i] = '\0';
	return (i);
}

int	ft_strlen_sep(char *str, char *sep)
{
	int	i;

	i = 0;
	if (!str)
		return (0);
	if (!sep)
		return (ft_strlen(str));
	while (str + i && str + i != sep)
		i++;
	return (i);
}
