/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_non_builtins.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 04:49:38 by sabakar-          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/08/26 11:15:09 by sabakar-         ###   ########.fr       */
=======
/*   Updated: 2024/08/23 16:34:19 by kipouliq         ###   ########.fr       */
>>>>>>> kilian_test_more
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

<<<<<<< HEAD
// int	ft_exec_non_builtins(t_token *node)
// {
// 	char	*la_path;
// 	char	**env_args;

// 	ft_shell()->exit_status = ft_check_redirections(node);
// 	env_args = env_lst_to_arr(&ft_shell()->env_lst);
// 	la_path = ft_check_path(node->contents[0], env_args);
// 	if (!la_path)
// 	{
// 		ft_putstr_fd("minishell: ", 2);
// 		ft_putstr_fd(node->contents[0], 2);
// 		// ft_putchar_fd(node->contents[1], 2);
// 		ft_putstr_fd(" :", 2);
// 		return (ft_print_err(CMD_ERR), -1);
// 	}
// 	if (execve(la_path, node->contents, ft_shell()->env_args) == -1)
// 		return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
// 	// dprintf(2, "LAUNCHING EXECVE CMD %s\n", node->contents[0]);
// // 	if (execve(la_path, node->contents, env_args) == -1)
// // 		return (perror("bash: execve: "), ft_free(env_args),
// 			// exit(255), -1);
// 	// return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL),
// 	// 	exit(255), -1);
// 	return (-1);
// }

int    ft_exec_non_builtins(t_token *node)
{
    char    *la_path;
    char    **env_args;
    
    printf("NODE %s REDIR LST =====\n", node->contents[0]);
    print_redir_lst(&node->redirections);
    // ft_shell()->exit_status = ft_check_redirections(node);
        //   ------> THIS LINE
    env_args = env_lst_to_arr(&ft_shell()->env_lst);
    la_path = ft_check_path(node->contents[0], env_args);
    if (!la_path)
    {
        ft_putstr_fd("minishell: ", 2);
        ft_putstr_fd(node->contents[0], 2);
        // ft_putchar_fd(node->contents[1], 2);
        ft_putstr_fd(" :", 2);
        return (ft_print_err(CMD_ERR), exit(1), -1);
    }
    // dprintf(2, "LAUNCHING EXECVE CMD %s\n", node->contents[0]);
    if (execve(la_path, node->contents, env_args) == -1)
        return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
    // // if (execve(la_path, node->contents, env_args) == -1)
    // //     return (perror("bash: execve: "), ft_free(env_args),
    // //         exit(255), -1);
    // // return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL),
    // //     exit(255), -1);
    return (0);
}
=======
int	ft_exec_non_builtins(t_token *node)
{
	char	*la_path;
	char	**env_args;
    int     exit_status;

	env_args = env_lst_to_arr(&ft_shell()->env_lst);
	la_path = ft_check_path(node->contents[0], env_args, &exit_status);
	if (!la_path)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(exit_status), -1);
	// dprintf(2, "launching cmd %s\n", node->contents[0]);
	if (execve(la_path, node->contents, ft_shell()->env_args) == -1)
		return (perror("bash: execve: "), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
	return (0);
}
>>>>>>> kilian_test_more
