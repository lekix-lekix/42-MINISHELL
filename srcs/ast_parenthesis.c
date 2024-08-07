/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parenthesis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:19:53 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/01 18:17:16 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*create_par_lst(t_token **lst)
{
	t_token	*par;
	t_token	*dup_lst;

	if (!*lst)
		return (NULL);
	dup_lst = (*lst)->next;
	if (!dup_lst)
		return (NULL);
	par = find_closing_par(lst);
	if (!par)
		par = find_right_par(lst);
	dup_lst = lst_dup(&dup_lst, par);
	return (dup_lst);
}

int	check_par_lst(t_token **par_lst)
{
	t_token	*current;

	current = *par_lst;
	while (current)
	{
		if (current->type == CMD || is_a_redir_operator(current)
			|| is_a_token_operator(current))
			return (1);
		current = current->next;
	}
	return (0);
}

void	set_is_in_par(t_ast *root, int flag)
{
	if (root->left)
		set_is_in_par(root->left, flag);
	root->is_in_par = flag;
	if (root->right)
		set_is_in_par(root->right, flag);
}

t_ast	*handle_par(t_token **lst, t_ast **tree, int *insert_node)
{
	t_token	*par_lst;
	t_ast	*par_tree;

    (void) insert_node;
	par_lst = create_par_lst(lst);
	if (!par_lst || !check_par_lst(&par_lst))
	{
		print_char_syntax_error(")");
		return (NULL);
	}
	par_tree = build_ast(&par_lst, insert_node);
	if (!par_tree)
		return (NULL);
	set_is_in_par(par_tree, 1);
	if (!*tree)
		*tree = par_tree;
	if (check_tree_syntax(&par_tree) == -1)
		return (NULL);
	return (par_tree);
}
