/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/15 20:29:24 by lekix            ###   ########.fr       */
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
	else if (((*last_operator)->node_type == AND
			&& ft_shell()->exit_status == 0)
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
		if (current->token_node->contents)
			printf("current = %s\n", current->token_node->contents[0]);
		else
			printf("op_node type = %d\n", current->node_type);
		if (current->token_node->redirections)
			print_redir_lst(&current->token_node->redirections);
		current = current->next;
	}
}

int	init_exec_ast_data(t_ast **lst)
{
	(void)lst;
	ft_shell()->pids_num = 0;
	return (0);
}

void	check_next_op(t_ast *root, t_ast *to_find, t_ast **last_node_visited,
		int *pipe_after)
{
	if (root->left)
		check_next_op(root->left, to_find, last_node_visited, pipe_after);
	if (root->node_type == CMD)
		*last_node_visited = root;
	if (root->node_type == PIPE && last_node_visited
		&& *last_node_visited == to_find)
		*pipe_after = 1;
	if (root->right)
		check_next_op(root->right, to_find, last_node_visited, pipe_after);
}

int	pipe_after_par(t_ast *node)
{
	t_ast	*root;
	t_ast	*last_node_visited;
	int		pipe_after;

	pipe_after = 0;
	last_node_visited = NULL;
	root = ft_shell()->exec_tree;
	check_next_op(root, node, &last_node_visited, &pipe_after);
	printf("pipe after = %d\n", pipe_after);
	return (pipe_after);
}

// void	check_last_op(t_ast *root, t_ast *to_find, t_ast **last_node_visited,
// 		int *pipe_after)
// {
// 	if (root->left)
// 		check_last_op(root->left, to_find, last_node_visited, pipe_after);
// 	if (root->node_type == CMD)
// 		*last_node_visited = root;
// 	if (root->right)
// 		check_last_op(root->right, to_find, last_node_visited, pipe_after);
// 	if (root->node_type == PIPE && last_node_visited
// 		&& *last_node_visited == to_find)
// 		*pipe_after = 1;
// }

// int	pipe_before_par(t_ast *node)
// {
// 	t_ast	*root;
// 	t_ast	*last_node_visited;
// 	int		pipe_after;

// 	pipe_after = 0;
// 	last_node_visited = NULL;
// 	root = ft_shell()->exec_tree;
// 	check_last_op(root, node, &last_node_visited, &pipe_after);
// 	printf("pipe before = %d\n", pipe_after);
// 	return (pipe_after);
// }

int	init_pipe(int *pipe_fds)
{
	if (pipe(pipe_fds) == -1)
		return (perror("bash: pipe"), gbg_coll(NULL, ALL, FLUSH_ALL), exit(-1),
			-1);
	return (0);
}

void	set_pipe_redir_out(t_ast *root, int *par_pipe, char *pipe_filename)
{
	t_redir	*pipe_redir;

	if (root->left)
		set_pipe_redir_out(root->left, par_pipe, pipe_filename);
	if (root->node_type == CMD)
	{
		pipe_redir = create_redir_node(REDIR_OUTPUT, pipe_filename);
		add_front_redir_node(&root->token_node->redirections, pipe_redir);
		root->token_node->pipe_redir[1][0] = par_pipe[0];
		root->token_node->pipe_redir[1][1] = par_pipe[1];
	}
	if (root->right)
		set_pipe_redir_out(root->right, par_pipe, pipe_filename);
}

int	set_pipe_redir(t_ast *par_sub_tree, int *par_pipe, t_ast *first_par_node)
{
	t_redir	*input_redir;
	int		pipes_i;

	set_pipe_redir_out(par_sub_tree, par_pipe, "pipe");
	pipes_i = ft_shell()->pids_num;
    printf("pids num = %d\n", pipes_i);
	if (pipes_i > 0)
	{
        printf("first par node content = %s\n", first_par_node->token_node->contents[0]);
		input_redir = create_redir_node(REDIR_INPUT, "pipe");
        printf("input redir name = %s\n", input_redir->filename);
		add_front_redir_node(&first_par_node->token_node->redirections,
			input_redir);
		first_par_node->token_node->pipe_redir[0][0] = ft_shell()->pipes[pipes_i
			- 1][0];
		first_par_node->token_node->pipe_redir[0][1] = ft_shell()->pipes[pipes_i
			- 1][1];
	}
    printf("BEFORE REDIR OUT\n");
    print_redir_lst(&first_par_node->redirections);
	return (0);
}

int	iterate_exec_ast_lst(t_ast **lst)
{
	t_ast	*current;
	t_ast	*par_sub_tree;
	int		cmd_nb;
	int		par_pipe_out[2];
	t_ast	*last_par_node;

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
            printf("current = %s\n", current->token_node->contents[0]);
			par_sub_tree = find_top_node(&current);
			get_last_node_tree(par_sub_tree, &last_par_node);
            printf("current = %s\n", current->token_node->contents[0]);
			if (pipe_after_par(last_par_node))
			{
				init_pipe(par_pipe_out);
				set_pipe_redir(par_sub_tree, par_pipe_out, current);
			}
			printf("==== AST LST PAR === \n");
			print_ast_lst(lst);
			printf("=======\n");
			printf("PAR SUB TREE ====\n");
			print_tree(&par_sub_tree);
			printf("=========\n");
			set_is_in_par(par_sub_tree, 0);
			set_next_null(par_sub_tree);
			ft_shell()->exec_in_par = 1;
			printf("start exec PAR start\n");
			// ft_start_exec(&par_sub_tree, par_pipe_out);
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

	printf("START EXEC\n");
	root = *tree;
	exec_lst = NULL;
	if (root->node_type == CMD)
	{
		// printf("init only child\n");
		return (init_only_child_no_fork(root->token_node));
	}
	last_op = NULL;
	ft_start_exec_tree(root, &exec_lst, &last_op);
	if (exec_lst)
		iterate_exec_ast_lst(&exec_lst);
	return (0);
}
