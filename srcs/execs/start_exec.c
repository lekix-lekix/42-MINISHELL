/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/25 15:23:40 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

void ft_reset_ports(bool piped, t_minishell *data)
{
	if (piped)
		return ;
	dup2(data->stdin, 0);
	dup2(data->stdout, 1);
}

void	ft_exec_pipe_child(t_ast *node, int pids[],
		t_cmd_pipe_directions direction, t_minishell *data)
{
	int	la_status;

	if (direction == P_CMD_LEFT)
	{
		close(pids[0]);
		dup2(pids[1], STDOUT_FILENO);
		close(pids[1]);
	}
	else if (direction == P_CMD_RIGHT)
	{
		close(pids[1]);
		dup2(pids[0], STDIN_FILENO);
		close(pids[0]);
	}
	la_status = ft_start_exec(&node, data);
	exit(la_status);
}

int	ft_get_la_status(int la_status)
{
	if (WIFSIGNALED(la_status))
		return (128 + WTERMSIG(la_status));
	return (WEXITSTATUS(la_status));
}

int	ft_exec_pipe(t_ast *node, t_minishell *data)
{
	int	pids[2];
	int	pipe_l;
	int	pipe_r;
	int	la_status;

	la_status = 0;
	pipe(pids);
	pipe_l = fork();
	if (pipe_l == 0)
		ft_exec_pipe_child(node->left, pids, P_CMD_LEFT, data);
	pipe_r = fork();
	if (pipe_r == 0)
		ft_exec_pipe_child(node->right, pids, P_CMD_RIGHT, data);
	(close(pids[0]), close(pids[1]));
	(waitpid(pipe_l, &la_status, 0), waitpid(pipe_r, &la_status, 0));
	return (ft_get_la_status(la_status));
}

int	ft_check_cmds(t_token *token_node, t_minishell *data)
{
	// char	**la_args;
	int		le_satus;

	// la_args = ft_split(token_node->content, 32);
	// int x = -1;
	// while (token_node->contents[++x])
		// printf("-------The splited cmd: %s --------;\n", token_node->contents[x]);
	// print_lst(&token_node);
	if (ft_is_builtin(token_node->contents[0]))
	{
		le_satus = ft_check_redirections(token_node->redirections, data);
		if (le_satus != ENO_SUCCESS)
			return (ft_reset_ports(false, data), ENO_GENERAL);
		printf("We passed the redirections\n");
		le_satus = ft_exec_builtins(token_node->contents, data, token_node->redirections);
		return (ft_reset_ports(false, data), le_satus);
	}
	else
		return (ft_exec_non_builtins(token_node->contents, data, token_node->redirections));
}

int	ft_start_exec(t_ast **tree, t_minishell *data)
{
	t_ast	*nodes;
	int		la_status;

	la_status = -1;
	nodes = *tree;
	if (!nodes)
		return (1);

	if (nodes->node_type == PIPE)
		return (ft_exec_pipe(nodes, data));
	else if (nodes->node_type == AND)
	{
		la_status = ft_start_exec(&nodes->left, data);
		if (la_status == ENO_SUCCESS)
			return (ft_start_exec(&nodes->right, data));
		return (la_status);
	}
	else if (nodes->node_type == OR)
	{
		la_status = ft_start_exec(&nodes->left, data);
		if (la_status == ENO_SUCCESS)
			return (la_status);
		return (ft_start_exec(&nodes->right, data));
	}
	else
		return (ft_check_cmds(nodes->token_node, data));
	return (ENO_GENERAL);
}
