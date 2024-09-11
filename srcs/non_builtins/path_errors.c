/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_errors.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:34:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 20:33:24 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_msh_error(char *err, char *cmd)
{
	char	*str;

	str = ft_join("minishell: ", cmd);
	str = ft_join(str, ": ");
	str = ft_join(str, err);
	str = ft_join(str, "\n");
	write(2, str, ft_strlen(str));
	return (0);
}

char	*dot_error(void)
{
	write(2, "bash: .: filename argument required\n", 37);
	write(2, ".: usage: . filename [arguments]\n", 34);
	return (ft_exit_close(127), NULL);
}
