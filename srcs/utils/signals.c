/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:33:38 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/20 12:22:27 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_sigint_handler(int num)
{
	(void)num;
	ft_shell()->exit_status = 130;
	if (ft_shell()->signint_child)
	{
		write(2, "\n", 1);
		ft_shell()->signint_child = false;
	}
	else
	{
		if (waitpid(-1, NULL, WNOHANG) == -1 && errno == ECHILD)
		{
			write(2, "\n", 1);
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	ft_sigquit_handler(int num)
{
	(void)num;
	write(2, "Quit (core dumped)\n", 19);
	ft_shell()->exit_status = 131;
}

void	ft_init_signals(void)
{
	ft_shell()->heredoc_sigint = false;
	ft_shell()->signint_child = false;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}

void	ft_sigint_handler_heredoc(int num)
{
	if (num == SIGINT)
	{
		close(STDIN_FILENO);
		ft_shell()->heredoc_sigint = true;
		ft_shell()->exit_status = 130;
	}
}
