/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:20:35 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/19 12:57:39 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	add_ast_lst(t_ast **ast_lst, t_ast *node)
{
	t_ast	*current;

	current = *ast_lst;
	if (!current)
	{
		*ast_lst = node;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = node;
}

int	ast_lst_size(t_ast **lst)
{
	t_ast	*current;
	int		i;

	i = 0;
	current = *lst;
	while (current)
	{
		i++;
		current = current->next;
	}
	return (i);
}

t_ast	*find_top_node(t_ast **lst)
{
	t_ast	*current;
	t_ast	*top_node;

	current = *lst;
	top_node = NULL;
	while (current)
	{
		if (current->node_type == AND || current->node_type == OR || current->node_type == PIPE)
			top_node = current;
		if (!current->is_in_par)
			return (top_node);
		current = current->next;
	}
	return (top_node);
}

void	set_next_null(t_ast *root)
{
	if (root->left)
		set_next_null(root->left);
	root->next = NULL;
	if (root->right)
		set_next_null(root->right);
}

void	init_pids_tab(t_ast **lst)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		if (current->node_type == CMD)
			ft_shell()->pids_num += 1;
		current = current->next;
	}
	printf("pids tab size = %d\n", ft_shell()->pids_num);
	ft_shell()->pids = malloc(sizeof(pid_t) * ft_shell()->pids_num - 1);
	// if (!ft_shell()->pids || gbg_coll(ft_shell()->pids, ADD, PARSING))
	// 	(gbg_coll(NULL, ALL, FLUSH_ALL), exit(255));
}
