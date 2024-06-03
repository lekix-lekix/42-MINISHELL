/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_parenthesis.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 17:19:53 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/03 13:11:04 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*find_last_par(t_token **lst)
{
	t_token	*current;
	t_token	*par;

	current = *lst;
	par = NULL;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->type == PAR_RIGHT)
			par = current;
		current = current->next;
	}
	return (par);
}

t_token *find_closing_par(t_token **lst)
{
    t_token *current;

    current = (*lst)->next;
    if (!current)
        return (NULL);
    while (current)
    {
        if (current->type == PAR_LEFT)
            return (find_last_par(lst));
        if (current->type == PAR_RIGHT)
            return (current);
        current = current->next;
    }
    return (NULL);
}

t_token	*find_right_par(t_token **lst)
{
	t_token	*current;

	current = *lst;
	while (current)
	{
		if (current->type == PAR_RIGHT)
			return (current);
		current = current->next;
	}
	return (current);
}

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
    dup_lst = lst_dup(&dup_lst, par);
	return (dup_lst);
}

t_ast	*handle_par(t_token **lst, t_ast **tree, t_ast **root, int *insert_node)
{
	t_token	*par_lst;
	t_ast	*par_tree;

    printf("handle par function\n");
	par_lst = create_par_lst(lst);
    printf("handle par print_lst ====\n");
    print_lst(&par_lst);
    printf("=========================\n");
	if (!par_lst)
    {
        printf("NO PAR LST\n");
		return (NULL);
    }
	par_tree = build_ast(&par_lst, insert_node);
    printf("====\n");
    print_tree(&par_tree);
    printf("====\n");
	par_tree->is_in_par = 1;
    if (!*root)
    {
        *root = par_tree;
        *tree = par_tree;
    }
	return (par_tree);
}
