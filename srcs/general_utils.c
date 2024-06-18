/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   general_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/11 07:14:54 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/11 07:35:23 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	ft_print_err(char *str)
{
	int		x;
	int		len;
	char	*new_str;

	len = ft_strlen(str);
	x = 0;
	new_str = (char *)malloc(sizeof(char) * len + 1);
	if (!new_str)
		return ;
	while (str[x])
	{
		new_str[x] = str[x];
		x++;
	}
	new_str[x] = '\n';
	write(2, new_str, len + 1);
	free(new_str);
}

void	ft_free(char **arr)
{
	int	x;

	x = 0;
	if (!arr)
		return ;
	while (arr[x])
	{
		free(arr[x]);
		arr[x] = NULL;
		x++;
	}
	free(arr);
	arr = NULL;
}