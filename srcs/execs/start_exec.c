/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/04 18:38:54 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"
int	iterate_exec_ast_lst(t_ast **lst, int *la_status);

void	ft_reset_ports(bool piped)
{
	if (piped)
		return ;
	dup2((ft_shell())->stdin, 0);
	dup2((ft_shell())->stdout, 1);
}

// void	ft_exec_pipe_child(t_ast *node, int pids[],
// 		t_cmd_pipe_directions direction)
// {
// 	int	la_status;

// 	if (direction == P_CMD_LEFT)
// 	{
// 		close(pids[0]);
// 		dup2(pids[1], STDOUT_FILENO);
// 		close(pids[1]);
// 	}
// 	else if (direction == P_CMD_RIGHT)
// 	{
// 		close(pids[1]);
// 		dup2(pids[0], STDIN_FILENO);
// 		close(pids[0]);
// 	}
// 	// la_status = ft_start_exec(&node);
// 	exit(la_status);
// }

int	ft_get_la_status(int la_status)
{
	if (WIFSIGNALED(la_status))
		return (128 + WTERMSIG(la_status));
	return (WEXITSTATUS(la_status));
}

// int	ft_exec_pipe(t_ast *node)
// {
// 	int	pids[2];
// 	int	la_status;
// 	int	pipe_l;
// 	int	pipe_r;

// 	// printf("CALLED\n");
// 	la_status = 0;
// 	pipe(pids);
// 	pipe_l = fork();
// 	// dprintf(2, "FORKING\n");
// 	if (pipe_l == 0)
// 		ft_exec_pipe_child(node->left, pids, P_CMD_LEFT);
// 	else
// 	{
// 		pipe_r = fork();
// 		// dprintf(2, "FORKING\n");
// 		if (pipe_r == 0)
// 			ft_exec_pipe_child(node->right, pids, P_CMD_RIGHT);
// 		else
// 		{
// 			(close(pids[0]), close(pids[1]));
// 			waitpid(pipe_r, &la_status, 0);
// 		}
// 		(ft_shell())->heredoc_sigint = false;
// 		(close(pids[0]), close(pids[1]));
// 		(waitpid(pipe_l, &la_status, 0));
// 	}
// 	return (la_status);
// }

int	ft_check_cmds(t_token *token_node)
{
	int	le_satus;

	// int	x;
	// x = -1;
	// while (token_node->contents[++x])
	// 	printf("The token is: %s\n", token_node->contents[x]);
	if (!token_node->contents)
	{
		// printf("no content\n");
		le_satus = ft_check_redirections(token_node->redirections);
		return (ft_reset_ports(false), (le_satus && ENO_GENERAL));
	}
	if (ft_is_builtin(token_node->contents[0]))
	{
		// printf("It's a builtin\n");
		le_satus = ft_check_redirections(token_node->redirections);
		if (le_satus != ENO_SUCCESS)
			return (ft_reset_ports(false), ENO_GENERAL);
		le_satus = ft_exec_builtins(token_node->contents, (ft_shell()));
		return (ft_reset_ports(false), le_satus);
	}
	else
	{
		// printf("It's a non builtin\n");
		return (ft_exec_non_builtins(token_node->contents,
				token_node->redirections));
	}
}

// int	ft_start_exec(t_ast **tree)
// {
// 	t_ast	*nodes;
// 	int		la_status;

// 	la_status = -1;
// 	nodes = *tree;
// 	if (!nodes)
// 		return (1);
// 	if (nodes->node_type == PIPE)
// 	{
// 		// printf("It's a pipe\n");
// 		return (ft_exec_pipe(nodes));
// 	}
// 	else if (nodes->node_type == AND)
// 	{
// 		// printf("It's an AND\n");
// 		la_status = ft_start_exec(&nodes->left);
// 		if (la_status == ENO_SUCCESS)
// 			return (ft_start_exec(&nodes->right));
// 		return (la_status);
// 	}
// 	else if (nodes->node_type == OR)
// 	{
// 		// printf("It's a OR\n");
// 		la_status = ft_start_exec(&nodes->left);
// 		if (la_status == ENO_SUCCESS)
// 			return (la_status);
// 		return (ft_start_exec(&nodes->right));
// 	}
// 	else
// 	{
// 		// printf("It's a simple cmd\n");
// 		return (ft_check_cmds(nodes->token_node));
// 	}
// 	return (ENO_GENERAL);
// }

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

void	ft_start_exec_tree(t_ast *root, t_ast **to_exec, int *la_status, int *first_exec)
{
	if (root->left /* && root->left->visited == 0 */)
	{
		printf("going left\n");
		ft_start_exec_tree(root->left, to_exec, la_status, first_exec);
	}
	if (root->node_type == CMD)
	{
		printf("adding node to lst\n");
		add_ast_lst(to_exec, root);
	}
	// root->visited = 1
	if ((root->node_type == AND || root->node_type == OR) && !root->is_in_par)
	{
		if (*first_exec) 
		{
			printf("first exec\n");
			iterate_exec_ast_lst(to_exec, la_status);
			*to_exec = NULL ;
			*first_exec = 0;
		}
		else if (!*first_exec)
		{
			printf("second exec\n");
			if (root->node_type == AND && la_status == 0)
			{
				*la_status = iterate_exec_ast_lst(to_exec, la_status);
				*to_exec = NULL ;
			}
			if (root->node_type == OR && la_status != 0)
			{
				*la_status = iterate_exec_ast_lst(to_exec, la_status);
				*to_exec = NULL ;
			}
		}
	}
	
	if (root->right/*  && root->right->visited == 0 */)
	{
		printf("going right\n");
		ft_start_exec_tree(root->right, to_exec, la_status, first_exec);
	}
	if ((root->node_type == AND || root->node_type == OR) && !root->is_in_par)
	{
		if (*first_exec) 
		{
			printf("first exec\n");
			iterate_exec_ast_lst(to_exec, la_status);
			*to_exec = NULL ;
			*first_exec = 0;
		}
		else if (!*first_exec)
		{
			printf("second exec\n");
			printf("la status = %d\n", *la_status);
			printf("The node type: %u\n", root->node_type);
			if (root->node_type == AND && *la_status == 0)
			{
				printf("went throught\n");
				*la_status = iterate_exec_ast_lst(to_exec, la_status);
				*to_exec = NULL ;
			}
			if (root->node_type == OR && *la_status != 0)
			{
				*la_status = iterate_exec_ast_lst(to_exec, la_status);
				*to_exec = NULL ;
			}
		}
	}
}

void	print_ast_lst(t_ast **lst)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		printf("current ast = %s\n", current->token_node->contents[0]);
		current = current->next;
	}
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

int	init_first_child(t_ast *node)
{
	printf("FIRST CHILD = %s!\n", node->token_node->contents[0]);
	return (0);
}
int	init_middle_child(t_ast *node)
{
	printf("MIDDLE CHILD = %s!\n", node->token_node->contents[0]);
	return (0);
}

int	init_last_child(t_ast *node)
{
	printf("LAST CHILD = %s!\n", node->token_node->contents[0]);
	return (0);
}

int	iterate_exec_ast_lst(t_ast **lst, int *la_status)
{
	t_ast	*current;
	int		lst_size;

	current = *lst;
	lst_size = ast_lst_size(lst);
	while (current)
	{
		if (current == *lst)
			init_first_child(current);
		else if (!current->next)
			init_last_child(current);
		else
			init_middle_child(current);
		current = current->next;
	}
	*la_status = 1;
	return (1);
}

void	init_pids_tab(t_ast **tree)
{
	t_ast *current;

	current = *tree;
	while (current)
	{
		if (current->node_type == CMD)
			ft_shell()->pids_num += 1;
		current = current->next;
	}
	printf("pids tab size = %d\n", ft_shell()->pids_num);
	ft_shell()->pids = malloc(sizeof(pid_t) * ft_shell()->pids_num - 1);
	// if (!ft_shell()->pids || gbg_coll(ft_shell()->pids, ADD, PARSING))
	// 	(gbg_coll(NULL, ALL, FLUSH_ALL), exit(255));
}

int	ft_start_exec(t_ast **tree)
{
	t_ast *root;
	t_ast *to_exec;
	int la_status;
	int	first_exec;
	// int cmd_list;

	root = *tree;
	to_exec = NULL;
	if (root->node_type == CMD)
	{
		la_status = ft_check_cmds(root->token_node);
		return (la_status);
	}
	ft_shell()->pids_num = 0;
	first_exec = 1;
	init_pids_tab(&to_exec);
	print_ast_lst(&to_exec);
	ft_start_exec_tree(root, &to_exec, &la_status, &first_exec);
	return (la_status);
}