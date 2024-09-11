/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_print.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:57:52 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 18:40:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	print_contents(t_ast *node)
{
	int	i;

	i = -1;
	while (node->token_node->contents[++i])
		printf("contents[%d] = %s\n", i, node->token_node->contents[i]);
}

void	traverse_print(t_ast *node, int level)
{
	if (node->left)
		traverse_print(node->left, level + 1);
	printf("----NODE-------\n");
	printf("node content = %s, level = %d, type = %d\n",
		node->token_node->content, level, node->node_type);
	printf("in par = %d\n", node->is_in_par);
	if (node->token_node->contents)
		print_contents(node);
	if (node->token_node->redirections)
		print_redir_lst(&node->token_node->redirections);
	if (node->right)
		traverse_print(node->right, level + 1);
}

void	print_tree(t_ast **tree)
{
	t_ast	*node;

	node = *tree;
	traverse_print(node, 0);
}
