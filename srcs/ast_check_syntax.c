/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_check_syntax.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:48:38 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/05 18:11:56 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_ast	*get_first_node_tree(t_ast *root)
{
	if (root->left)
		return (get_first_node_tree(root->left));
	return (root);
	if (root->right)
		return (get_first_node_tree(root->right));
}

void	get_last_node_tree(t_ast *root, t_ast **node)
{
	if (root->left)
		get_last_node_tree(root->left, node);
	*node = root;
	if (root->right)
		get_last_node_tree(root->right, node);
}

int	check_par_syntax(t_token **lst)
{
	t_token	*current;
	int		par;

	current = *lst;
	if (!current)
		return (-1);
	par = 0;
	while (current)
	{
		if (current->type == PAR_LEFT)
			par--;
		else if (current->type == PAR_RIGHT)
			par++;
		current = current->next;
	}
	if (par < 0)
		return (print_char_syntax_error("("));
	if (par > 0)
		return (print_char_syntax_error(")"));
	return (0);
}

void	check_tree_syntax_recursive(t_ast *root, t_ast **syntax_node,
		int *syntax_flag, int *node_nb)
{
	if (root->left)
		check_tree_syntax_recursive(root->left, syntax_node, syntax_flag,
			node_nb);
	if (root->node_type == CMD)
		*syntax_flag += 1;
	if (root->node_type > CMD)
		*syntax_flag -= 1;
	if (*syntax_flag > 1 || *syntax_flag < 0)
	{
		*syntax_node = root;
		return ;
	}
	*node_nb += 1;
	if (root->right)
		check_tree_syntax_recursive(root->right, syntax_node, syntax_flag,
			node_nb);
}

int	check_tree_syntax(t_ast **tree)
{
	t_ast	*root;
	t_ast	*error_node;
	int		syntax_flag;
	int		node_nb;

	root = *tree;
	syntax_flag = 0;
	node_nb = 0;
	error_node = NULL;
	error_node = get_first_node_tree(root);
	if (error_node->node_type != CMD)
		return (print_ast_syntax_error(error_node));
	check_tree_syntax_recursive(root, &error_node, &syntax_flag, &node_nb);
	error_node = NULL;
	if (error_node && !error_node->is_in_par)
		return (print_ast_syntax_error(error_node));
	get_last_node_tree(root, &error_node);
	if (error_node->node_type != CMD && !error_node->is_in_par)
		return (print_ast_syntax_error(error_node));
	return (0);
}
