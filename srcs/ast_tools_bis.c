/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tools_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:48:15 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/10 11:48:37 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	parse_tree_size(t_ast *root, int *size)
{
	if (root->left)
		parse_tree_size(root->left, size);
	*size += 1;
	if (root->right)
		parse_tree_size(root->right, size);
}

int	tree_size(t_ast **tree)
{
	t_ast	*root;
	int		size;

	root = *tree;
	size = 0;
	parse_tree_size(root, &size);
	return (size);
}
