/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_check_syntax_bis.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 13:59:56 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/20 14:03:57 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	traverse_count_pipes(t_ast *root, int *pipes)
{
	if (root->left)
		traverse_count_pipes(root->left, pipes);
	if (root->node_type == PIPE)
		*pipes += 1;
	if (root->right)
		traverse_count_pipes(root->right, pipes);
}

int	ft_count_pipes(t_ast **tree)
{
	int	pipes;

	pipes = 0;
	if (!*tree)
		return (0);
	traverse_count_pipes(*tree, &pipes);
	return (pipes);
}

int	check_pipes_par_syntax(t_token **lst)
{
	t_token	*current;
	int		open_par;

	open_par = 0;
	current = *lst;
	if ((*lst)->type == PIPE)
	{
		write(2, "minishell: syntax error near unexpected token `|'\n", 51);
		return (-1);
	}
	while (current)
	{
		if (current->type == PAR_LEFT)
			open_par = 1;
		else if (current->type == PAR_RIGHT)
			open_par = 0;
		if (open_par && current->type == PIPE)
		{
			write(2, "minishell: syntax error near unexpected token `|'\n", 51);
			return (-1);
		}
		current = current->next;
	}
	return (0);
}
