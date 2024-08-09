/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:07:00 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/09 17:30:09 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_check_option(char *sr)
{
	int	x;

	x = 0;
	if (sr[0] != '-')
		return (0);
	x++;
	while (sr[x])
	{
		if (sr[x] == 'n')
			return (1);
		x++;
	}
	return (0);
}

int	ft_exec_echo(char **args)
{
	int	x;
	int	opt;

	x = 1;
	opt = 0;
    dprintf(2, "launching echo\n");
	while (args[x] && ft_check_option(args[x]))
	{
		opt = 1;
		x++;
	}
	while (args[x])
	{
		ft_putstr_fd(args[x], 1);
		if (args[x + 1])
			write(1, " ", 1);
		x++;
	}
	if (opt == 0)
		ft_putstr_fd("\n", 1);
	return (0);
}
