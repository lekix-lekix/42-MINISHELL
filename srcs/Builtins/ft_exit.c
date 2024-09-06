/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:18:16 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/05 11:11:25 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	ft_err(char *sr_err)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(sr_err, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	// gbg_coll(NULL, ALL, FLUSH_ALL);
}

static bool	ft_isnumber(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

static void	ft_skip_spaces_and_get_sign(char *s, int *i, int *sign)
{
	while (s[*i] && s[*i] == ' ')
		(*i)++;
	if (s[*i] == '+' || s[*i] == '-')
	{
		if (s[*i] == '-')
			*sign *= -1;
		(*i)++;
	}
}

static int	ft_exittoi(char *sr)
{
	int					i;
	int					sign;
	unsigned long long	result;

	i = 0;
	sign = 1;
	ft_skip_spaces_and_get_sign(sr, &i, &sign);
	if (!ft_isnumber(sr + i))
		return (ft_err(sr), 2);
	result = 0;
	while (sr[i])
	{
		result = (result * 10) + (sr[i] - '0');
		if (result > LONG_MAX)
			(ft_err(sr));
		i++;
	}
	return ((result * sign) % 256);
}

void	ft_exit(char **args)
{
	int	exit_s;

	exit_s = (ft_shell())->exit_status;
	if (args[1])
	{
		if (args[2] && ft_isnumber(args[1]))
		{
			ft_putstr_fd("minishell: exit: too many arguments\n", 2);
            return ;
			// exit(ENO_GENERAL);
		}
		else
			exit_s = ft_exittoi(args[1]);
	}
    close(ft_shell()->ft_stdin);
    close(ft_shell()->ft_stdout);
    gbg_coll(NULL, ALL, FLUSH_ALL);
	exit(exit_s);
}
