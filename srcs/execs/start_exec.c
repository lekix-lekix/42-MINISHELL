/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/10 18:01:14 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

int		iterate_exec_ast_lst(t_ast **lst);
int		ft_start_exec(t_ast **tree);

void	ft_reset_ports(bool piped)
{
	if (piped)
		return ;
	dup2((ft_shell())->stdin, 0);
	dup2((ft_shell())->stdout, 1);
}

int	ft_get_la_status(int la_status)
{
	if (WIFSIGNALED(la_status))
		return (128 + WTERMSIG(la_status));
	return (WEXITSTATUS(la_status));
}

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
		la_status = ft_exec_non_builtins(token_node->contents,
				token_node->redirections);
	return (0);
}

int	check_operator_exec_bis(t_ast *op_node, t_ast **exec_lst)
{
	if ((op_node->node_type == AND && ft_shell()->exit_status == 0)
		|| (op_node->node_type == OR && ft_shell()->exit_status != 0))
	{
		printf("NODE TYPE = %d\n", op_node->node_type);
		iterate_exec_ast_lst(exec_lst);
		*exec_lst = NULL;
		return (0);
	}
	else
	{
		printf("cant exec !\n");
		*exec_lst = NULL;
		return (1);
	}
}

int	check_operator_exec(t_ast *root, t_ast **exec_lst, int *first_exec,
		t_ast **last_op)
{
	// printf("exit status = %d\n", ft_shell()->exit_status);
	if (!root->is_in_par && (root->node_type == AND || root->node_type == OR))
	{
		printf("checking operator node = %d\n", root->node_type);
		if (*first_exec)
		{
			iterate_exec_ast_lst(exec_lst);
			*exec_lst = NULL;
			*first_exec = 0;
			return (1);
		}
		else if (*last_op)
		{
			printf("checking last op\n");
			check_operator_exec_bis(*last_op, exec_lst);
			*last_op = NULL;
			return (1);
		}
		else if (root->node_type == AND || root->node_type == OR)
			return (check_operator_exec_bis(root, exec_lst));
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

void	ft_start_exec_tree(t_ast *root, t_ast **exec_lst, int *first_exec,
		int level, t_ast **last_op)
{
	if (root->left)
		ft_start_exec_tree(root->left, exec_lst, first_exec, level + 1,
			last_op);
	if (root->node_type == CMD)
	{
		printf("adding node = %s\n", root->token_node->contents[0]);
		add_ast_lst(exec_lst, root);
	}
	if (root->is_in_par && (root->node_type == AND || root->node_type == OR))
    {
        printf("adding op node type = %d\n", root->node_type);
		add_ast_lst(exec_lst, root);
    }
	check_operator_exec(root, exec_lst, first_exec, last_op);
	if (root->right && level == 0)
		*last_op = root;
	if (root->right)
		ft_start_exec_tree(root->right, exec_lst, first_exec, level + 1,
			last_op);
	check_operator_exec(root, exec_lst, first_exec, last_op);
}

int	init_only_child_no_fork(t_token *node)
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

int	init_only_child(t_token *node)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), perror("bash: fork"), exit(255),
			-1);
	if (pid == 0)
	{
		if (!node->contents[0])
		{
			ft_shell()->exit_status = ft_check_redirections(node->redirections);
			return (ft_reset_ports(false), 0);
		}
		if (ft_is_builtin(node->contents[0]))
			exit(ft_exec_builtins(node->contents, ft_shell()));
		ft_exec_non_builtins(node->contents, node->redirections);
	}
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status))
		ft_shell()->exit_status = WEXITSTATUS(status);
	printf("exit status = %d\n", ft_shell()->exit_status);
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
	while (++i < pids_num)
	{
		waitpid(pids[i], &status, WUNTRACED);
		if (WIFEXITED(status))
			ft_shell()->exit_status = WEXITSTATUS(status);
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

int	iterate_exec_ast_lst(t_ast **lst)
{
	t_ast	*current;
	t_ast	*par_sub_tree;
	int		**pipes;
	int		cmd_nb;
    

	// print_ast_lst(lst);
	current = *lst;
	cmd_nb = ast_list_size(lst);
	ft_shell()->pids_num = 0;
	printf("nb of cmds = %d\n", cmd_nb);
	if (!cmd_nb)
		return (0);
	if (cmd_nb == 1)
		return (init_only_child(current->token_node));
	pipes = alloc_pipes_tab(cmd_nb);
	ft_shell()->pids = alloc_pids_tab(lst);
	while (current)
	{
		printf("executing cmd = %s\n", current->token_node->contents[0]);
		if (current->is_in_par)
		{
			par_sub_tree = find_top_node(&current);
            if (par_sub_tree)
                printf("top node type = %d\n", par_sub_tree->node_type);
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
	wait_all_pids(ft_shell()->pids);
	printf("exit status = %d\n", ft_shell()->exit_status);
	return (0);
}

int	ft_start_exec(t_ast **tree)
{
	t_ast	*root;
	t_ast	*exec_lst;
    t_ast   *last_op;
	int		first_exec;

	printf("START EXEC\n");
	root = *tree;
	exec_lst = NULL;
	if (root->node_type == CMD)
	{
		// printf("init only child\n");
		return (init_only_child_no_fork(root->token_node));
	}
	first_exec = 1;
    last_op = NULL;
	ft_start_exec_tree(root, &exec_lst, &first_exec, 0, &last_op);
	if (first_exec)
		iterate_exec_ast_lst(&exec_lst);
	return (0);
}
