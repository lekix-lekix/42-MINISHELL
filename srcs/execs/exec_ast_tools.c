/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_ast_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/08 14:20:35 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/11 14:53:52 by lekix            ###   ########.fr       */
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

void    test_tree_size(t_ast *root, int *tree_size)
{
    if (root->left)
        test_tree_size(root->left, tree_size);
    *tree_size += 1;
    if (root->right)
        test_tree_size(root->right, tree_size);
}

t_ast	*find_top_node(t_ast **lst)
{
	t_ast	*current;
	int		lst_size;
    int     tree_size;

    lst_size = ast_list_size(lst);
    if (lst_size == 1)
        return (*lst);
    current = *lst;
    while (current)
    {
        tree_size = 0;
        test_tree_size(current, &tree_size);
        if (tree_size == lst_size)
            return (current);
        else
            current = current->next;
    }
    return (NULL);
}

// t_ast	*find_top_node(t_ast **lst)
// {
// 	t_ast	*current;
// 	t_ast	*top_node;

// 	current = *lst;
// 	top_node = NULL;
// 	while (current)
// 	{
// 		if (current->node_type == AND || current->node_type == OR
// 			|| current->node_type == PIPE)
// 			top_node = current;
// 		if (current->node_type == CMD && current->next
// 			&& current->next->node_type == CMD)
// 			return (top_node);
// 		current = current->next;
// 	}
// 	return (top_node);
// }

void	set_next_null(t_ast *root)
{
	if (root->left)
		set_next_null(root->left);
	root->next = NULL;
	if (root->right)
		set_next_null(root->right);
}

void	print_ast_lst(t_ast **lst)
{
	t_ast	*current;

	current = *lst;
	printf("START AST LST ========\n");
	while (current)
	{
		if (current->token_node->contents)
			printf("current = %s %s\n", current->token_node->contents[0],
				current->token_node->contents[1]);
		else
			printf("op_node type = %d\n", current->node_type);
		if (current->token_node->redirections)
        {
			print_redir_lst(&current->token_node->redirections);
            printf("pipe 0 = %d\n", current->token_node->pipe_redir[0]);
            printf("pipe 1 = %d\n", current->token_node->pipe_redir[1]);
        }
		current = current->next;
	}
	printf("END ==========\n");
}
