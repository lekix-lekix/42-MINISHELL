/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:07:00 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 15:44:21 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_check_option(char *sr)
{
	int	x;

	x = 1;
	if (sr[0] != '-')
		return (0);
	while (sr[x] == 'n')
		x++;
	if (ft_is_space(sr[x]) || !sr[x])
		return (1);
	return (0);
}

int	ft_exec_echo(char **args)
{
	int	i;
	int	opt;
	int	arr_len;

	opt = 0;
	arr_len = get_arr_len(args);
	if (arr_len == 1)
		return (printf("\n"), 0);
	if (arr_len == 2 && ft_check_option(args[1]))
		return (0);
	i = 0;
	while(ft_check_option(args[++i]))
		opt = 1;
	while (args[i] && i < arr_len - 1)
	{
		printf("%s ", args[i]);
		i++;
	}
	if (!opt)
		printf("%s\n", args[i]);
	else
		printf("%s", args[i]);
	return (0);
}
