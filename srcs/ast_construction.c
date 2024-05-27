/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_construction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:48:33 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/27 19:05:35 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	traverse_print(t_ast *node, int level)
{
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

int	parse_insert_op_token_node(t_ast *root, t_ast *node, int level)
{
	// printf("node = %s\n level = %d\n", node->token_node->content, level);
	if (root->left)
		parse_insert_op_token_node(root->left, node, level + 1);
	if (root->right)
		parse_insert_op_token_node(root->right, node, level + 1);
	// printf("root = %s type = %d, node = %s type = %d\n",
	// root->token_node->content, root->node_type, node->token_node->content,
	// node->node_type);
	if (root->node_type > node->node_type)
	{
		if (!root->left)
			root->left = node;
		else
			root->right = node;
		return (0);
	}
	if (root->node_type <= node->node_type && level == 0)
		return (1);
	// add error case
	return (-1);
}

void	insert_operator_token_node(t_ast **tree, t_ast *node)
{
	t_ast	*root;

	root = *tree;
	if (!root)
	{
		*tree = node;
		return ;
	}
	if (parse_insert_op_token_node(root, node, 0) == 1)
	{
		*tree = node;
		node->left = root;
	}
}

t_token	*find_operator_token(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return (NULL);
	while (current)
	{
		if (current->type > 0)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_ast	*create_ast_node(t_token *node)
{
	t_ast	*new_node;

	new_node = malloc(sizeof(t_ast));
	if (!new_node || gbg_coll(new_node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	new_node->token_node = node;
	new_node->left = NULL;
	new_node->right = NULL;
	new_node->is_in_par = 0;
	new_node->node_type = node->type;
	return (new_node);
}

void	consume_node(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	if (!current || !node)
		return ;
	printf("_______\n");
	print_lst(lst);
	printf("_______\n");
	while (current)
	{
		// printf("current = %s\n", current->content);
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

t_token	*find_last_par(t_token **lst)
{
	t_token	*current;
	t_token	*par;

	printf("=====\n");
	print_lst(lst);
	printf("=====\n");
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
	// printf("par = %s\n", par->content);
	return (par);
}

t_ast	*build_operator_tree(t_token **lst)
{
	t_ast	*tree;
	t_ast	*right;
	t_ast	*new_node;
	t_token	*current;
	t_token	*last_par;

	tree = NULL;
	right = NULL;
	last_par = NULL;
	current = *lst;
	while (current)
	{
		printf("------\n");
		print_lst(lst);
		printf("------\n");
		current = find_operator_token(&current);
		if (current && current->type == PAR_LEFT)
		{
			last_par = find_last_par(&current);
			current = find_operator_token(&last_par->next);
			if (current)
				printf("last par = %s\n", last_par->content);
		}
		if (!current)
			break ;
		printf("currrrrent= %s\n", current->content);
		new_node = create_ast_node(current);
		consume_node(lst, new_node->token_node);
		if (tree && new_node->node_type < tree->node_type)
			insert_operator_token_node(&right, new_node);
		else
			insert_operator_token_node(&tree, new_node);
		current = current->next;
	}
	tree->right = right;
	return (tree);
}

// (1 && 2) || (3 && 4)
// ((1 && 2) || 4) | 4

int	parse_insert_cmd_node(t_ast *root, t_ast *cmd_node, int level)
{
	// printf("root = %s, cmd-node = %s level = %d\n",
	// root->token_node->content, cmd_node->token_node->content, level);
	if (root->left)
	{
		if (parse_insert_cmd_node(root->left, cmd_node, level + 1) == 0)
			return (0);
	}
	if (!root->left && (root->node_type > cmd_node->node_type
			|| (root->node_type > CMD && cmd_node->is_in_par)))
	{
		root->left = cmd_node;
		return (0);
	}
	if (root->right)
	{
		if (parse_insert_cmd_node(root->right, cmd_node, level + 1) == 0)
			return (0);
	}
	if (!root->right && (root->node_type > cmd_node->node_type
			|| (root->node_type > CMD && cmd_node->is_in_par)))
	{
		root->right = cmd_node;
		return (0);
	}
	return (-1);
}

t_token	*lst_dup(t_token **lst, t_token *node)
{
	t_token	*lst_cpy;
	t_token	*current_cpy;
	t_token	*current;

	current = *lst;
	lst_cpy = NULL;
	while (current != node)
	{
		current_cpy = malloc(sizeof(t_token));
		if (!current_cpy || gbg_coll(current_cpy, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		current_cpy->type = current->type;
		current_cpy->content = current->content;
		current_cpy->next = NULL;
		insert_node_lst(&lst_cpy, current_cpy);
		current = current->next;
	}
	return (lst_cpy);
}

t_token	*create_par_lst(t_token **lst)
{
	t_token	*par;
	t_token	*dup_lst;

	if (!*lst)
        return (NULL);
    par = find_last_par(lst);
    dup_lst = lst_dup(lst, par);
	return (dup_lst);
}

t_token	*find_right_par(t_token **lst)
{
	t_token	*current;

	current = *lst;
	// printf("====\n");
	// print_lst(lst);
	// printf("====\n");
	while (current)
	{
		// printf("find right par current = %s, next = %s\n", current->content,
		// current->next->content);
		if (current->type == PAR_RIGHT)
			return (current);
		current = current->next;
	}
	return (current);
}

t_ast	*handle_par(t_token **lst)
{
	t_token	*par_lst;
	t_ast	*par_tree;

    printf("par lst =====\n");
	par_lst = create_par_lst(lst);
	if (!par_lst)
		return (NULL);
    printf("par lst =====\n");
	par_tree = build_ast(&par_lst);
	par_tree->is_in_par = 1;
	print_tree(&par_tree);
	return (par_tree);
}

void	build_cmd_tree(t_ast **tree, t_token **lst)
{
	t_token	*current;
	t_ast	*cmd_node;
	t_ast	*root;

	current = *lst;
	root = *tree;
	while (current)
	{
		printf("current = %s\n", current->content);
		if (current->type == PAR_LEFT)
		{
			cmd_node = handle_par(&current->next);
			parse_insert_cmd_node(root, cmd_node, 0);
			current = find_right_par(&current);
			// printf("---\n");
			// print_lst(&current);
			// printf("---\n");
			continue ;
		}
		else
			cmd_node = create_ast_node(current);
		parse_insert_cmd_node(root, cmd_node, 0);
		current = current->next;
	}
}

t_ast	*build_ast(t_token **lst)
{
	t_ast	*tree;

	// printf("******\n");
	// print_lst(lst);
	// printf("******\n");
	tree = build_operator_tree(lst);
	printf("operator tree built\n");
	// printf("******\n");
	// print_lst(lst);
	// printf("******\n");
	build_cmd_tree(&tree, lst);
	return (tree);
}

// cmd1 | cmd2 || cmd3 | cmd4
// (1 && 2) | (3 && 4)
