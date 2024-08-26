/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/08/22 14:03:24 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
	while (current)
	{
		if (current->is_in_par)
		{
			par_pid = handle_par_exec(&current);
			ft_lstadd_back(&ft_shell()->pids, create_lst_node(&par_pid));
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
