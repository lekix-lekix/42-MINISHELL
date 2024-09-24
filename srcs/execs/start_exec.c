/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/24 13:49:47 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// t_ast *get_next_node(t_ast **lst)
// {
//     t_ast *current;

//     current = *lst;
//     while (current)
//     {
//         if (current->node_type == PAR_RIGHT)
//             return (current->next);
//         current = current->next;
//     }
//     return (NULL);
// }

int	iterate_exec_ast_lst(t_ast **lst)
{
	t_ast	*current;
	int		cmd_nb;
	int		par_pid;

	par_pid = -1;
	current = *lst;
	cmd_nb = ast_list_size(lst);
	if (!cmd_nb)
		return (0);
	if (cmd_nb == 1)
		return (init_only_child_no_fork(current->token_node));
	while (current)
	{
		// printf("current before exec par = %s\n",
			// current->token_node->contents[0]);
		if (current->is_in_par)
		{
			dprintf(2, "first current node par = %s %s\n",
				current->token_node->contents[0],
				current->token_node->contents[1]);
			par_pid = handle_par_exec(&current);
			ft_lstadd_back(&ft_shell()->pids, create_lst_node(&par_pid));
			// current = get_next_node(&current);
			// current = current->next;
            current = NULL;
			continue ;
		}
		prep_exec_child(current);
		current = current->next;
	}
	close_wait(par_pid);
	return (0);
}

int	ft_start_exec(t_ast **tree)
{
	t_ast	*root;
	t_ast	*exec_lst;
	t_ast	*last_op;

	root = *tree;
	ft_shell()->exec_tree = *tree;
	exec_lst = NULL;
	if (root->node_type == CMD)
		return (init_only_child_no_fork(root->token_node));
	last_op = NULL;
	ft_start_exec_tree(root, &exec_lst, &last_op);
	if (exec_lst)
		iterate_exec_ast_lst(&exec_lst);
	return (0);
}
