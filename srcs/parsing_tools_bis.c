/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_tools_bis.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 15:05:08 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/10 15:09:51 by kipouliq         ###   ########.fr       */
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

