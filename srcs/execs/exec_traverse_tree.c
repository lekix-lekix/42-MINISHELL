/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_traverse_tree.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:20:09 by lekix             #+#    #+#             */
/*   Updated: 2024/08/02 14:39:07 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_next_op(t_ast *root, t_ast *to_find, t_ast **last_node_visited,
		int *pipe_after)
{
	if (root->left)
		check_next_op(root->left, to_find, last_node_visited, pipe_after);
	if (root->node_type == CMD)
		*last_node_visited = root;
	if (root->node_type == PIPE && *last_node_visited
		&& *last_node_visited == to_find)
		*pipe_after = 1;
	if (root->right)
		check_next_op(root->right, to_find, last_node_visited, pipe_after);
}

int	check_last_operator(t_ast **last_operator)
{
	if (!*last_operator)
		return (1);
	else if (((*last_operator)->node_type == AND
			&& ft_shell()->exit_status == 0)
		|| ((*last_operator)->node_type == OR && ft_shell()->exit_status != 0))
		return (1);
	else
		return (0);
}

int	check_operator_exec(t_ast *root, t_ast **exec_lst, t_ast **last_operator)
{
	if (!root->is_in_par)
	{
		if (*last_operator == NULL || check_last_operator(last_operator))
		{
			// printf("can exec !\n");
			iterate_exec_ast_lst(exec_lst);
			*last_operator = root;
			*exec_lst = NULL;
			return (0);
		}
		else
		{
			// printf("cant exec !\n");
			*exec_lst = NULL;
			return (1);
		}
	}
	return (0);
}

void	ft_start_exec_tree(t_ast *root, t_ast **exec_lst, t_ast **last_op)
{
	if (root->left)
		ft_start_exec_tree(root->left, exec_lst, last_op);
	if (root->is_in_par && (root->node_type == AND || root->node_type == OR
			|| root->node_type == PIPE) && check_last_operator(last_op))
		add_ast_lst(exec_lst, root);
	if (root->node_type == CMD)
		add_ast_lst(exec_lst, root);
	if (root->node_type == AND || root->node_type == OR)
		check_operator_exec(root, exec_lst, last_op);
	if (root->right)
		ft_start_exec_tree(root->right, exec_lst, last_op);
	if (root->node_type == AND || root->node_type == OR)
		check_operator_exec(root, exec_lst, last_op);
}
