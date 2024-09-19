/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/05 21:33:38 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/19 12:06:58 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static void	ft_sigint_handler(int num)
{
	(void)num;
	if (ft_shell()->signint_child)
	{
		printf("\n");
		ft_shell()->signint_child = false;
		ft_shell()->heredoc_sigint = true;
	}
	else
	{
		if (waitpid(-1, NULL, WNOHANG) == -1 && errno == ECHILD)
		{
			printf("\n");
			rl_replace_line("", 0);
			rl_on_new_line();
			rl_redisplay();
		}
	}
}

void	ft_sigquit_handler(int num)
{
	(void)num;
	ft_putstr_fd("Quit: 3\n", 1);
}

void	ft_init_signals(void)
{
	// struct termios	term;

	// term = ft_shell()->original_term;
	// tcsetattr(STDIN_FILENO, TCSANOW, &term);
	ft_shell()->heredoc_sigint = false;
	ft_shell()->signint_child = false;
	signal(SIGINT, ft_sigint_handler);
	signal(SIGQUIT, SIG_IGN);
}
    