/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_construction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:48:33 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/04 14:32:21 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	traverse_print(t_ast *node, int level)
{
	// printf("node content = %s, level = %d\n", node->token_node->content,
	// level);
	if (node->left)
		traverse_print(node->left, level + 1);
	printf("node content = %s, level = %d\n", node->token_node->content, level);
	if (node->right)
		traverse_print(node->right, level + 1);
}

void	print_tree(t_ast **tree)
{
	t_ast	*node;

	node = *tree;
	traverse_print(node, 0);
}

void	consume_node(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	if (!current || !node)
		return ;
	if (current == node)
	{
		*lst = current->next;
		gbg_coll(node->content, PARSING, FREE);
		return ;
	}
	while (current)
	{
		if (current == node)
		{
			prev->next = current->next;
			gbg_coll(node->content, PARSING, FREE);
			// gbg_coll(node, PARSING, FREE); // bizarre ? probable leak
		}
		prev = current;
		current = current->next;
	}
}

void	create_consume_insert_node(t_token **lst, t_token *node, t_ast **tree,
		t_ast **tree_right)
{
	t_ast	*new_node;

	printf("inserting node = %s\n", node->content);
	new_node = create_ast_node(node);
	consume_node(lst, new_node->token_node);
	if (*tree && new_node->node_type < (*tree)->node_type)
		insert_operator_token_node(tree_right, new_node);
	else
		insert_operator_token_node(tree, new_node);
}

void	syntax_error(t_token *node)
{
	printf("error syntax : %s\n", node->content);
}

t_ast	*build_operator_tree(t_token **lst)
{
	t_ast	*tree;
	t_ast	*right;
	t_token	*current;
	t_token	*par;

	tree = NULL;
	right = NULL;
	current = *lst;
	while (current)
	{
		current = find_operator_token(&current);
		if (current && current->type == PAR_LEFT)
		{
			par = find_closing_par(&current);
			if (!par)
				return (syntax_error(current), NULL);
			current = par;
			continue ;
		}
		if (!current)
			break ;
		create_consume_insert_node(lst, current, &tree, &right);
		current = current->next;
	}
	if (tree && right)
		tree->right = right;
	return (tree);
}

int	build_cmd_tree(t_ast **tree, t_token **lst)
{
	t_token	*current;
	t_ast	*cmd_node;
	// t_ast	*root;
	int		insert_node;

	current = *lst;
	if (!current)
		return (-1);
	// root = *tree;
	insert_node = 1;
	while (current && current->type < 5)
	{
        printf("current cmd = %s\n", current->content);
		if (current->type == PAR_LEFT)
		{
			cmd_node = handle_par(&current, tree, /* &root,  */&insert_node);
            if (!cmd_node)
            {
                *tree = NULL;
                return (-1);
                // printf("no cmd node\n");
                // return (syntax_error(find_closing_par(&current)), -1);
            }
			if (insert_node)
            {    
				if (insert_cmd_node(tree, cmd_node) == -1)
                    return (syntax_error(get_first_node_tree(cmd_node)->token_node), -1);
            }
			printf("coucou\n");
			current = find_closing_par(&current);
			continue ;
		}
		else
			cmd_node = create_ast_node(current);
		printf("return (= %d\n", insert_cmd_node(tree, cmd_node));
        printf("cmd tree building -------------\n");
        print_tree(tree);
        printf("-----------------------------\n");
		current = current->next;
	}
	return (0);
}

t_ast	*build_ast(t_token **lst, int *insert_node)
{
	t_ast	*tree;

	// printf("coucou\n");
	tree = build_operator_tree(lst);
	if (!tree)
    {
        printf("no operator tree\n");
		// return (NULL);
    }
	// printf("====\n");
	// print_lst(lst);
	// printf("====\n");
	if (build_cmd_tree(&tree, lst) == -1)
		*insert_node = 0;
	return (tree);
}

// cmd1 | cmd2 || cmd3 | cmd4
// (1 && 2) | (3 && 4)
// (1 && 2) || (3 && 4)
// ((1 && 2) || 4) | 4
// ((1 && 2) || (3 && 4) || 5) SIGSEV
// (((1 || 2) || 3) || 4)
