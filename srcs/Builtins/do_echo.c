/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:07:00 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 17:28:13 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static int	ft_check_option(char *sr)
{
	int	x;

	x = 0;
	// printf("THE STR: %s\n", sr);
	if (sr[0] != '-')
		return (0);
	x++;
	while (sr[x])
	{
		if (sr[1] == 'n' && (ft_is_space(1 + 1) || sr[1 + 1] == '\0'))
			return (1);
		x++;
	}
	return (0);
}

int	ft_exec_echo(char **args)
{
	int		x;
	int		opt;
	int		args_len;
	char	**option;

	x = 1;
	opt = 0;
	// printf("THE ARGS: %s\n", args[x]);
	if (!args || !args[x])
		return (printf("\n"), 0);
	args_len = get_arr_len(args);
	if (args_len > 2)
	{
		while (args[x] != NULL && ft_check_option(args[x]))
		{
			// printf("THE ARGS: %s\n", args[x]);
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
		if (!opt)
			ft_putstr_fd("\n", 1);
	}
	else
	{
		option = ft_split(args[x], 32);
		if (!option)
			return (/* gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), */(printf("\n")), -1);
		if (option)
		{
			if (ft_check_option(option[0]))
			{
				opt = 1;
				while (option[x])
				{
					ft_putstr_fd(option[x], 1);
					if (option[x + 1])
						write(1, " ", 1);
					x++;
				}
			}
			else
			{
				while (args[x])
				{
					ft_putstr_fd(args[x], 1);
					if (args[x + 1])
						write(1, " ", 1);
					x++;
				}
				if (!opt)
					ft_putstr_fd("\n", 1);
			}
		}
		// printf("THE ARGS: %s\n", args[x]);
	}
	// printf("The tem: %s", args[1]);
	return (0);
}
