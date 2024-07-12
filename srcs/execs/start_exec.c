/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/12 16:04:22 by kipouliq         ###   ########.fr       */
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

// int	check_operator_exec_bis(t_ast *op_node, t_ast **exec_lst)
// {
// 	if ((op_node->node_type == AND && ft_shell()->exit_status == 0)
// 		|| (op_node->node_type == OR && ft_shell()->exit_status != 0))
// 	{
// 		printf("NODE TYPE = %d\n", op_node->node_type);
// 		iterate_exec_ast_lst(exec_lst);
// 		*exec_lst = NULL;
// 		return (0);
// 	}
// 	else
// 	{
// 		printf("cant exec !\n");
// 		*exec_lst = NULL;
// 		return (1);
// 	}
// }

int	check_last_operator(t_ast **last_operator)
{
    if (!*last_operator)
        return (1);
	else if (((*last_operator)->node_type == AND && ft_shell()->exit_status == 0)
		|| ((*last_operator)->node_type == OR && ft_shell()->exit_status != 0))
		return (1);
	else
		return (0);
}

int	check_operator_exec(t_ast *root, t_ast **exec_lst, t_ast **last_operator)
{
	if (!root->is_in_par)
	{
		// printf("checking operator node = %d, last exit status = %d\n",
			// root->node_type, ft_shell()->exit_status);
		// if (*last_operator)
			// printf("last op node type = %d\n", (*last_operator)->node_type);
		if (*last_operator == NULL || check_last_operator(last_operator))
		{
			printf("can exec !\n");
			iterate_exec_ast_lst(exec_lst);
			*last_operator = root;
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
	return (0);
}

void	ft_start_exec_tree(t_ast *root, t_ast **exec_lst, t_ast **last_op)
{
	if (root->left)
		ft_start_exec_tree(root->left, exec_lst, last_op);
	if (root->is_in_par && check_last_operator(last_op))
	{
		printf("adding par node type = %d\n", root->node_type);
		add_ast_lst(exec_lst, root);
	}
	else if (!root->is_in_par && root->node_type == CMD)
	{
		printf("adding node = %s\n", root->token_node->contents[0]);
		add_ast_lst(exec_lst, root);
	}
	if ((root->node_type == AND || root->node_type == OR))
		check_operator_exec(root, exec_lst, last_op);
	if (root->right)
		ft_start_exec_tree(root->right, exec_lst, last_op);
	if (root->node_type == AND || root->node_type == OR)
		check_operator_exec(root, exec_lst, last_op);
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

// void	ft_start_exec_tree(t_ast *root, t_ast **exec_lst, int *first_exec,
// 		int level, t_ast **last_op)
// {
// 	if (root->left)
// 		ft_start_exec_tree(root->left, exec_lst, first_exec, level + 1,
// 			last_op);
// 	if (root->node_type == CMD)
// 	{
// 		printf("adding node = %s\n", root->token_node->contents[0]);
// 		add_ast_lst(exec_lst, root);
// 	}
// 	if (root->is_in_par && (root->node_type == AND || root->node_type == OR))
//     {
//         printf("adding op node type = %d\n", root->node_type);
// 		add_ast_lst(exec_lst, root);
//     }
// 	check_operator_exec(root, exec_lst, first_exec, last_op);
// 	if (root->right && level == 0)
// 		*last_op = root;
// 	if (root->right)
// 		ft_start_exec_tree(root->right, exec_lst, first_exec, level + 1,
// 			last_op);
// 	check_operator_exec(root, exec_lst, first_exec, last_op);
// }

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
    printf("hey hey\n");
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status))
		ft_shell()->exit_status = WEXITSTATUS(status);
	printf("exit status = %d\n", ft_shell()->exit_status);
	return (0);
}

int	init_first_child(t_ast *node, int **pipes)
{
	pid_t	pid;
	int		pids_num;

	pids_num = ft_shell()->pids_num;
	// printf("FIRST CHILD\n");
	if (pipe(pipes[pids_num]) == -1)
		return (perror("bash: pipe"), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
	pid = fork();
	if (pid == -1)
		return (perror("bash: fork"), gbg_coll(NULL, ALL, FLUSH_ALL), exit(255),
			-1);
	if (pid == 0)
	{
		close(pipes[pids_num][0]);
		if (dup2(pipes[pids_num][1], STDOUT_FILENO) == -1)
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
		close(pipes[pids_num][1]);
		ft_check_cmds(node->token_node);
	}
	ft_shell()->pids[0] = pid;
	ft_shell()->pids_num += 1;
	return (0);
}

int	init_middle_child(t_ast *node, int **pipes)
{
	pid_t	pid;

	if (pipe(pipes[ft_shell()->pids_num]) == -1)
		return (perror("bash: pipe"), gbg_coll(NULL, ALL, FLUSH_ALL), -1);
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

	pid = fork();
	if (pid == -1)
		return (perror("bash: fork"), gbg_coll(NULL, ALL, FLUSH_ALL), exit(255),
			-1);
	if (pid == 0)
	{
		if (dup2(pipes[ft_shell()->pids_num - 1][0], STDIN_FILENO))
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
		close(pipes[ft_shell()->pids_num - 1][0]);
		close(pipes[ft_shell()->pids_num - 1][1]);
		ft_check_cmds(node->token_node);
	}
	close(pipes[ft_shell()->pids_num - 1][0]);
	close(pipes[ft_shell()->pids_num - 1][1]);
	ft_shell()->pids[ft_shell()->pids_num] = pid;
	ft_shell()->pids_num += 1;
	return (0);
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

int	init_exec_ast_data(t_ast **lst)
{
	int	cmd_nb;

	cmd_nb = ast_list_size(lst);
	ft_shell()->pids_num = 0;
	return (0);
}

int	iterate_exec_ast_lst(t_ast **lst)
{
	t_ast	*current;
	t_ast	*par_sub_tree;
	int		cmd_nb;

	current = *lst;
	cmd_nb = ast_list_size(lst);
	if (!cmd_nb)
		return (0);
	if (cmd_nb == 1)
    {
        printf("only child\n");
		return (init_only_child_no_fork(current->token_node));
    }
	if (ft_shell()->exec_in_par)
		ft_shell()->exec_in_par = 0;
	else
		init_exec_ast_data(lst);
	ft_shell()->pids = alloc_pids_tab(lst);
	while (current)
	{
		// printf("executing cmd = %s\n", current->token_node->contents[0]);
		if (current->is_in_par)
		{
			par_sub_tree = find_top_node(&current);
			// if (par_sub_tree)
			// 	printf("top node type = %d\n", par_sub_tree->node_type);
            printf("PAR SUB TREE ====\n");
            print_tree(&par_sub_tree);
            printf("=========\n");
			set_is_in_par(par_sub_tree, 0);
			set_next_null(par_sub_tree);
			ft_shell()->exec_in_par = 1;
			printf("start exec PAR start\n");
			ft_start_exec(&par_sub_tree);
			printf("=== END ===\n");
            current = current->next;
			continue ;
		}
		if (ft_shell()->pids_num == 0)
		{
			printf("first child\n");
			init_first_child(current, ft_shell()->pipes);
		}
		else if (ft_shell()->pids_num == cmd_nb - 1)
		{
			printf("last_child\n");
			init_last_child(current, ft_shell()->pipes);
		}
		else
		{
			printf("middle child\n");
			init_middle_child(current, ft_shell()->pipes);
		}
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
	t_ast	*last_op;
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
	ft_start_exec_tree(root, &exec_lst, &last_op);
	if (first_exec)
		iterate_exec_ast_lst(&exec_lst);
	return (0);
}
