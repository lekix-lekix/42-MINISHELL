/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/09 18:22:00 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int		iterate_exec_ast_lst(t_ast **lst, int *la_status);
int		ft_start_exec(t_ast **tree);

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

// int	ft_check_cmds(t_token *token_node)
// {
// 	int	la_status;

// 	// int	x;
// 	// x = -1;
// 	// while (token_node->contents[++x])
// 	// 	printf("The token is: %s\n", token_node->contents[x]);
// 	if (!token_node->contents)
// 	{
// 		// printf("no content\n");
// 		la_status = ft_check_redirections(token_node->redirections);
// 		return (ft_reset_ports(false), (la_status && ENO_GENERAL));
// 	}
// 	if (ft_is_builtin(token_node->contents[0]))
// 	{
// 		// printf("It's a builtin\n");
// 		la_status = ft_check_redirections(token_node->redirections);
// 		if (la_status != ENO_SUCCESS)
// 			return (ft_reset_ports(false), ENO_GENERAL);
// 		la_status = ft_exec_builtins(token_node->contents, ft_shell());
// 		return (ft_reset_ports(false), la_status);
// 	}
// 	else
// 	{
// 		// printf("It's a non builtin\n");
// 		return (ft_exec_non_builtins(token_node->contents,
// 				token_node->redirections));
// 	}
// }

int	ft_check_cmds(t_token *token_node)
{
	int	la_status;

	la_status = ft_check_redirections(token_node->redirections);
	if (la_status != ENO_SUCCESS)
		gbg_coll(NULL, ALL, FLUSH_ALL), exit(la_status);
	if (ft_is_builtin(token_node->contents[0]))
	{
		la_status = ft_exec_builtins(token_node->contents, ft_shell());
		exit(la_status);
	}
	else
	{
		la_status = ft_exec_non_builtins(token_node->contents,
				token_node->redirections);
	}
	return (0);
}

int	check_operator_exec(t_ast *root, t_ast **exec_lst, int *first_exec,
		int *la_status)
{
	if (!root->is_in_par && (root->node_type == AND || root->node_type == OR))
	{
		if (*first_exec)
		{
			printf("first exec = %d\n", *first_exec);
			iterate_exec_ast_lst(exec_lst, la_status);
			*exec_lst = NULL;
			*first_exec = 0;
			return (1);
		}
		else if ((root->node_type == AND && *la_status == 0)
			|| (root->node_type == OR && *la_status != 0))
		{
			printf("la status = %d\n", *la_status);
			iterate_exec_ast_lst(exec_lst, la_status);
			*exec_lst = NULL;
			return (1);
		}
	}
	return (0);
}

int	ast_list_size(t_ast **lst)
{
	t_ast	*current;
	int		i;

	current = *lst;
	i = 0;
	while (current)
	{
		current = current->next;
		i++;
	}
	return (i);
}

void	ft_start_exec_tree(t_ast *root, t_ast **exec_lst, int *la_status,
		int *first_exec)
{
	if (root->left)
		ft_start_exec_tree(root->left, exec_lst, la_status, first_exec);
	if (root->node_type == CMD)
		add_ast_lst(exec_lst, root);
	if (root->is_in_par && (root->node_type == AND || root->node_type == OR))
		add_ast_lst(exec_lst, root);
	check_operator_exec(root, exec_lst, first_exec, la_status);
	if (root->right)
		ft_start_exec_tree(root->right, exec_lst, la_status, first_exec);
	check_operator_exec(root, exec_lst, first_exec, la_status);
}

int	init_only_child(t_token *node)
{
	pid_t	pid;
	int		status;

	if (!node->contents[0])
	{
		ft_shell()->exit_status = ft_check_redirections(node->redirections);
		return (ft_reset_ports(false), 0);
	}
	if (ft_is_builtin(node->contents[0]))
		return (ft_exec_builtins(node->contents, ft_shell()));
	pid = fork();
	if (pid == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), perror("bash: fork"), exit(255),
			-1);
	if (pid == 0)
		ft_exec_non_builtins(node->contents, node->redirections);
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status))
		ft_shell()->exit_status = WEXITSTATUS(status);
	return (0);
}

int	init_first_child(t_ast *node, int **pipes)
{
	pid_t	pid;

	// printf("FIRST CHILD\n");
	if (pipe(pipes[0]) == -1)
		return (perror("bash: pipe"), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
	pid = fork();
	if (pid == -1)
		return (perror("bash: fork"), gbg_coll(NULL, ALL, FLUSH_ALL), exit(255),
			-1);
	if (pid == 0)
	{
		close(pipes[0][0]);
		if (dup2(pipes[0][1], STDOUT_FILENO) == -1)
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
		close(pipes[0][1]);
		ft_check_cmds(node->token_node);
	}
	ft_shell()->pids[0] = pid;
	ft_shell()->pids_num += 1;
	return (0);
}

int	init_middle_child(t_ast *node, int **pipes)
{
	pid_t	pid;

	// printf("MIDDLE CHILD, pids_num = %d\n", ft_shell()->pids_num);
	if (pipe(pipes[ft_shell()->pids_num]) == -1)
		return (perror("bash: pipe"), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
	// close(pipes[ft_shell()->pids_num][0]);
	pid = fork();
	if (pid == -1)
		return (perror("bash: fork: "), gbg_coll(NULL, ALL, FLUSH_ALL),
			exit(255), -1);
	if (pid == 0)
	{
		if (dup2(pipes[ft_shell()->pids_num - 1][0], STDIN_FILENO))
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
		if (dup2(pipes[ft_shell()->pids_num][1], STDOUT_FILENO) == -1)
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
		close(pipes[ft_shell()->pids_num - 1][0]);
		close(pipes[ft_shell()->pids_num - 1][1]);
		close(pipes[ft_shell()->pids_num][1]);
		ft_check_cmds(node->token_node);
	}
	close(pipes[ft_shell()->pids_num - 1][0]);
	close(pipes[ft_shell()->pids_num - 1][1]);
	ft_shell()->pids[ft_shell()->pids_num] = pid;
	ft_shell()->pids_num += 1;
	return (0);
}

int	init_last_child(t_ast *node, int **pipes)
{
	pid_t	pid;

	// printf("LAST CHILD, pids_num = %d\n", ft_shell()->pids_num);
	// if (pipe(pipes[ft_shell()->pids_num]) == -1)
	// 	return (perror("bash: pipe"), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
	pid = fork();
	if (pid == -1)
		return (perror("bash: fork"), gbg_coll(NULL, ALL, FLUSH_ALL), exit(255),
			-1);
	if (pid == 0)
	{
		// printf("pids_num = %d\n", ft_shell()->pids_num);
		if (dup2(pipes[ft_shell()->pids_num - 1][0], STDIN_FILENO))
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
		close(pipes[ft_shell()->pids_num - 1][0]);
		close(pipes[ft_shell()->pids_num - 1][1]);
		// close(pipes[ft_shell()->pids_num][0]);
		// close(pipes[ft_shell()->pids_num][1]);
		ft_check_cmds(node->token_node);
	}
	// close(pipes[ft_shell()->pids_num][0]);
	// close(pipes[ft_shell()->pids_num][1]);
	close(pipes[ft_shell()->pids_num - 1][0]);
	close(pipes[ft_shell()->pids_num - 1][1]);
	ft_shell()->pids[ft_shell()->pids_num] = pid;
	ft_shell()->pids_num += 1;
	return (0);
}

int	**alloc_pipes_tab(int size)
{
	int	**tab;
	int	i;
	int	j;

	i = -1;
	tab = malloc(sizeof(int *) * (size + 1));
	if (!tab || gbg_coll(tab, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	while (++i < size)
	{
		tab[i] = malloc(sizeof(int) * 2);
		if (!tab[i] || gbg_coll(tab[i], PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		j = -1;
		while (++j < 2)
			tab[i][j] = 0;
	}
	tab[i] = NULL;
	return (tab);
}

pid_t	*alloc_pids_tab(t_ast **lst)
{
	pid_t	*tab;

	// printf("ast_list_size = %d\n", ast_list_size(lst));
	tab = malloc(sizeof(pid_t) * (ast_list_size(lst) + 1));
	if (!tab || gbg_coll(tab, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (tab);
}

int	wait_all_pids(pid_t *pids)
{
	int	i;
	int	pids_num;
	int	status;

	i = -1;
	pids_num = ft_shell()->pids_num;
	// printf("wait pids_num = %d\n", pids_num);
	while (++i < pids_num)
	{
		// printf("wait i = %d, pid = %d\n", i, pids[i]);
		waitpid(pids[i], &status, WUNTRACED);
		// printf("exit status = %d\n", status);
	}
	return (0);
}

void	print_ast_lst(t_ast **lst)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		printf("current = %s\n", current->token_node->contents[0]);
		current = current->next;
	}
}

int	iterate_exec_ast_lst(t_ast **lst, int *la_status)
{
	t_ast	*current;
	t_ast	*par_sub_tree;
	int		**pipes;
	int		cmd_nb;
	int		i;

	// print_ast_lst(lst);
	current = *lst;
	cmd_nb = ast_list_size(lst);
	if (cmd_nb == 1)
	{
		printf("init only child\n");
		*la_status = 1;
		// *lst = NULL;
		return (init_only_child(current->token_node));
	}
	pipes = alloc_pipes_tab(cmd_nb);
	i = 0;
	while (current)
	{
		if (current->is_in_par)
		{
			par_sub_tree = find_top_node(&current);
			set_is_in_par(par_sub_tree, 0);
			set_next_null(par_sub_tree);
			ft_start_exec(&par_sub_tree);
			continue ;
		}
		if (current == *lst)
			init_first_child(current, pipes);
		else if (!current->next)
			init_last_child(current, pipes);
		else
			init_middle_child(current, pipes);
		current = current->next;
	}
	*la_status = 1;
	// *lst = NULL;
	return (1);
}

int	ft_start_exec(t_ast **tree)
{
	t_ast	*root;
	t_ast	*exec_lst;
	int		la_status;
	int		first_exec;

	root = *tree;
	exec_lst = NULL;
	if (root->node_type == CMD)
	{
		// printf("init only child\n");
		return (init_only_child(root->token_node));
	}
	ft_shell()->pids_num = 0;
	first_exec = 1;
	ft_start_exec_tree(root, &exec_lst, &la_status, &first_exec);
	ft_shell()->pids = alloc_pids_tab(&exec_lst);
	if (first_exec)
		la_status = iterate_exec_ast_lst(&exec_lst, &la_status);
	wait_all_pids(ft_shell()->pids);
	return (la_status);
}
