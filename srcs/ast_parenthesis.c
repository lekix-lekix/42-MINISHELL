/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parenthesis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:19:53 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/28 17:12:21 by kipouliq         ###   ########.fr       */
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

t_ast	*handle_par(t_token **lst, t_ast **tree, int *insert_node)
{
	t_token	*par_lst;
	t_ast	*par_tree;

	par_lst = create_par_lst(lst);
	if (!par_lst || !check_par_lst(&par_lst))
	{
		print_char_syntax_error(")");
		return (NULL);
	}
	par_tree = build_ast(&par_lst, insert_node);
	if (!par_tree)
		return (NULL);
	par_tree->is_in_par = 1;
	if (!*tree)
		*tree = par_tree;
	if (check_tree_syntax(&par_tree) == -1)
		return (NULL);
	return (par_tree);
}
