/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_construction.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:48:33 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/18 11:30:44 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	traverse_print(t_ast *node, int level)
// {	
//     if (!node) 
//         return;
//     if (node->left)
//         traverse_print(node->left, level + 1);
//     // ft_start_exec(&node);
//     printf("The cmd is: %s\n", node->token_node->content);
//     // printf("node content = %s, level = %d\n", node->token_node->content, level);
//     if (node->right)
//         traverse_print(node->right, level + 1);
// }

// void	print_tree(t_ast **tree)
// {
// 	t_ast	*node;
	
// 	node = *tree;
// 	traverse_print(node, 0);
// }

int	parse_insert_op_token_node(t_ast *root, t_ast *node, int level)
{
	// printf("node = %s\n level = %d\n", node->token_node->content, level);
	if (root->left)
		parse_insert_op_token_node(root->left, node, level + 1);
	if (root->right)
		parse_insert_op_token_node(root->right, node, level + 1);
    // printf("root = %s type = %d, node = %s type = %d\n", root->token_node->content, root->node_type, node->token_node->content, node->node_type);
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
    new_node->node_type = node->type;
	return (new_node);
}

void    consume_node(t_token **lst, t_token *node)
{
    t_token *current;
    t_token *prev;

    current = *lst;
    if (!current || !node)
        return ;
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

t_ast	*build_operator_tree(t_token **lst)
{
	t_ast	*tree;
    t_ast   *right;
	t_token	*current;
	t_ast	*new_node;

	tree = NULL;
    right = NULL;
	while (1)
	{
		current = find_operator_token(lst);
		if (!current)
			break ;
		new_node = create_ast_node(current);
        consume_node(lst, new_node->token_node);
        if (tree && new_node->node_type < tree->node_type)
            insert_operator_token_node(&right, new_node);
        else
            insert_operator_token_node(&tree, new_node);
        // tree->right = right;
	}
    // printf("we don't get here!\n");
	// if (tree && tree->right) // it crashs here!
    tree->right = right;
    // printf("nor fucking here!\n");
	return (tree);
}

int    parse_insert_cmd_node(t_ast *root, t_ast *cmd_node, int level)
{
	if (!root || !cmd_node)
        return -1;
    // printf("root = %s, cmd-node = %s level = %d\n", root->token_node->content, cmd_node->token_node->content, level);
    if (root->left)
    {
        if (parse_insert_cmd_node(root->left, cmd_node, level + 1) == 0)
            return (0);
    }
    if (!root->left && root->node_type > cmd_node->node_type)
    {
        root->left = cmd_node;
        return (0);
    }
    if (root->right)
    {
        if (parse_insert_cmd_node(root->right, cmd_node, level + 1) == 0)
            return (0);
    }
    if (!root->right && root->node_type > cmd_node->node_type)
    {
        root->right = cmd_node;
        return (0);
    }
    return (-1);
}

void    insert_command_node(t_ast **tree, t_token *cmd_node)
{
    t_ast *root;
    t_ast *new_cmd_node;
	int	result;

    root = *tree;
    new_cmd_node = create_ast_node(cmd_node);
	if (root == NULL)
        *tree = new_cmd_node;
    else 
	{
        result = parse_insert_cmd_node(root, new_cmd_node, 0);
        if (result == -1)
        {
            // If the command node could not be inserted, make a new root
            // This is because the tree can't handle just one cmd
            new_cmd_node->left = root;
            *tree = new_cmd_node;
        }
    }
    // parse_insert_cmd_node(root, new_cmd_node, 0); // It's crashs here!
    // printf("insert cmd = %d\n", parse_insert_cmd_node(root, new_cmd_node, 0));
}

void    build_cmd_tree(t_ast **tree, t_token **lst)
{
    t_token *current;

    current = *lst;
    while (current)
    {
        insert_command_node(tree, current);
        current = current->next;
    }
}

t_ast	*build_ast(t_token **lst)
{
	t_ast	*tree;

	tree = build_operator_tree(lst);
    // printf("We don't get here!\n");
    // print_tree(&tree);
    build_cmd_tree(&tree, lst);
	return (tree);
}
