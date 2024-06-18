/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_pwd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 12:20:35 by sabakar-          #+#    #+#             */
/*   Updated: 2024/05/29 12:27:50 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int ft_exec_pwd(void)
{
    char    *cwd;

    cwd = NULL;
    cwd = getcwd(cwd, 0); // This is a native function that would get the wd.
    if (!cwd)
        return (1);
    ft_putstr_fd(cwd, 1);
    ft_putstr_fd("\n", 1);
    return (0);
}

