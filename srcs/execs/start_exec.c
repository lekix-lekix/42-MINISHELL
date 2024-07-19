/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_exec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/05 05:02:14 by sabakar-          #+#    #+#             */
/*   Updated: 2024/07/19 17:55:13 by lekix            ###   ########.fr       */
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

int	close_pipes_lst(t_lst **pipe_lst, t_token *node)
{
	t_lst	*current;
	int		*pipe;

	current = *pipe_lst;
	if (!current)
		return (-1);
	while (current)
	{
		pipe = (int *)current->content;
		if (pipe[0] != -1)
		{
			if (node)
				printf("cmd = %s closing pipe[0] = %d\n", node->contents[0],
					pipe[0]);
			close(pipe[0]);
			pipe[0] = -1;
		}
		if (pipe[1] != -1)
		{
			if (node)
				printf("cmd = %s closing pipe[1] = %d\n", node->contents[0],
					pipe[1]);
			close(pipe[1]);
			pipe[1] = -1;
		}
		current = current->next;
	}
	return (0);
}

int	ft_check_cmds(t_token *token_node)
{
	int	la_status;

	la_status = ft_check_redirections(token_node);
	close_pipes_lst(&ft_shell()->pipes, token_node);
	if (la_status != ENO_SUCCESS)
		gbg_coll(NULL, ALL, FLUSH_ALL), exit(la_status);
	if (ft_is_builtin(token_node->contents[0]))
	{
		la_status = ft_exec_builtins(token_node->contents, ft_shell());
		exit(la_status);
	}
	else
		la_status = ft_exec_non_builtins(token_node);
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
	if (root->is_in_par && (root->node_type == AND || root->node_type == OR
			|| root->node_type == PIPE) && check_last_operator(last_op))
	{
		// printf("adding par node type = %d\n", root->node_type);
		add_ast_lst(exec_lst, root);
	}
	if (root->node_type == CMD)
	{
		// printf("adding node = %s\n", root->token_node->contents[0]);
		add_ast_lst(exec_lst, root);
	}
	if (root->node_type == AND || root->node_type == OR)
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
		ft_shell()->exit_status = ft_check_redirections(node);
		return (ft_reset_ports(false), 0);
	}
	if (ft_is_builtin(node->contents[0]))
		return (ft_exec_builtins(node->contents, ft_shell()));
	pid = fork();
	if (pid == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), perror("bash: fork"), exit(255),
			-1);
	if (pid == 0)
		ft_exec_non_builtins(node);
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
			ft_shell()->exit_status = ft_check_redirections(node);
			return (ft_reset_ports(false), 0);
		}
		if (ft_is_builtin(node->contents[0]))
			exit(ft_exec_builtins(node->contents, ft_shell()));
		ft_exec_non_builtins(node);
	}
	// printf("hey hey\n");
	waitpid(pid, &status, WUNTRACED);
	if (WIFEXITED(status))
		ft_shell()->exit_status = WEXITSTATUS(status);
	printf("exit status = %d\n", ft_shell()->exit_status);
	return (0);
}

pid_t	*alloc_pids_tab(t_ast **lst)
{
	pid_t	*tab;

	// printf("ast_list_size = %d\n", ast_list_size(lst));
	tab = malloc(sizeof(pid_t) * (ast_list_size(lst)));
	if (!tab || gbg_coll(tab, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	return (tab);
}

int	wait_all_pids(t_lst **pids)
{
	t_lst	*current;
	int		status;

	current = *pids;
	while (current)
	{
		waitpid(*(pid_t *)current->content, &status, WUNTRACED);
		if (WIFEXITED(status))
			ft_shell()->exit_status = WEXITSTATUS(status);
		current = current->next;
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
	return (pipe_after);
}

// void	check_last_op(t_ast *root, t_ast *to_find, t_ast **last_node_visited,
// 		int *pipe_after)x
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

void	set_pipe_redir_out(t_ast *node, int pipe_fd)
{
	t_redir	*output_redir;

	output_redir = create_redir_node(REDIR_OUTPUT, "pipe");
	add_front_redir_node(&node->token_node->redirections, output_redir);
	node->token_node->pipe_redir[1] = pipe_fd;
}

void	set_pipe_redir_in(t_ast *node, int pipe_fd)
{
	t_redir	*input_redir;

	input_redir = create_redir_node(REDIR_INPUT, "pipe");
	add_front_redir_node(&node->token_node->redirections, input_redir);
	node->token_node->pipe_redir[0] = pipe_fd;
}

int	*init_pipe(void)
{
	int	*pipe_fds;

	pipe_fds = malloc(sizeof(int) * 2);
	if (!pipe_fds || gbg_coll(pipe_fds, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	if (pipe(pipe_fds) == -1)
		return (perror("bash: pipe"), gbg_coll(NULL, ALL, FLUSH_ALL), exit(-1),
			NULL);
	return (pipe_fds);
}

void	set_pipe_redir_out_tree(t_ast *root, int out_pipe, char *pipe_filename)
{
	t_redir	*pipe_redir;

	if (root->left)
		set_pipe_redir_out_tree(root->left, out_pipe, pipe_filename);
	if (root->node_type == CMD)
	{
		pipe_redir = create_redir_node(REDIR_OUTPUT, pipe_filename);
		add_front_redir_node(&root->token_node->redirections, pipe_redir);
		root->token_node->pipe_redir[1] = out_pipe;
	}
	if (root->right)
		set_pipe_redir_out_tree(root->right, out_pipe, pipe_filename);
}

// int	set_pipe_redir_par(t_ast *par_sub_tree, int *par_pipe,
// 		t_ast *first_par_node)
// {
// 	int	pipes_i;

// 	set_pipe_redir_out_tree(par_sub_tree, par_pipe[1], "pipe");
// 	pipes_i = ft_shell()->pids_num;
// 	if (pipes_i > 0)
// 	{
// 		set_pipe_redir_in(first_par_node, par_pipe[0]);
// 	}
// 	// if (&first_par_node->redirections)
// 	// 	print_redir_lst(&first_par_node->redirections);
// 	return (0);
// }

t_lst	*create_lst_node(void *mem_addr)
{
	t_lst	*lst_node;

	lst_node = malloc(sizeof(t_lst));
	if (!lst_node || gbg_coll(lst_node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	lst_node->content = mem_addr;
	lst_node->next = NULL;
	return (lst_node);
}

int	exec_child(t_ast *node)
{
	pid_t	*pid;
	t_lst	*pid_node;

	if (node->token_node->redirections)
		print_redir_lst(&node->token_node->redirections);
	pid = malloc(sizeof(pid_t));
	if (!pid || gbg_coll(pid, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	// printf("executing child : %s\n", node->token_node->contents[0]);
	*pid = fork();
	if (*pid == -1)
		return (perror("bash: fork: "), gbg_coll(NULL, ALL, FLUSH_ALL),
			exit(255), -1);
	if (*pid == 0)
		ft_check_cmds(node->token_node);
	pid_node = create_lst_node(pid);
	ft_lstadd_back(&ft_shell()->pids, pid_node);
	ft_shell()->pids_num += 1;
	return (0);
}

int	close_pipe_redir_in(t_ast *node)
{
	t_redir	*redirection;

	redirection = node->token_node->redirections;
	if (!redirection)
		return (-1);
	while (redirection)
	{
		if (redirection->redir_type == REDIR_INPUT
			&& ft_strcmp(redirection->filename, "pipe") == 0)
		{
			dprintf(2, "closing pipe in, return (value = %d\n",
				close(node->token_node->pipe_redir[0]));
		}
		redirection = redirection->next;
	}
	return (0);
}

void	close_pipe_redir_out(t_ast *node)
{
	t_redir	*redirection;

	redirection = node->token_node->redirections;
	if (!redirection)
		return ;
	while (redirection)
	{
		if (redirection->redir_type == REDIR_OUTPUT
			&& ft_strcmp(redirection->filename, "pipe") == 0)
		{
			dprintf(2, "closing pipe out, return (value = %d\n",
				close(node->token_node->pipe_redir[1]));
		}
		redirection = redirection->next;
	}
}

void	close_pipes(t_ast *node)
{
	close_pipe_redir_in(node);
	close_pipe_redir_out(node);
	// close(node->token_node->pipe_redir[0]);
	// close(node->token_node->pipe_redir[1]);
}

void	set_pipe_redir_in_par(t_ast **in_par_lst, int pipe_fd)
{
	t_ast	*current;

	current = *in_par_lst;
	while (current && current->is_in_par)
	{
		set_pipe_redir_in(current, pipe_fd);
		current = current->next;
	}
}

int	prep_exec_child(t_ast *to_exec)
{
	int		*pipe_fds;
	t_lst	*pipe_node;
	t_ast	*last_node;

	last_node = NULL;
	pipe_fds = NULL;
	printf("prepping cmd : %s\n", to_exec->token_node->contents[0]);
	if (to_exec->next)
	{
		// printf("lst size = %d\n", lst_size);
		pipe_fds = init_pipe();
		dprintf(2, "cmd = %s pipe %d\n", to_exec->token_node->contents[0],
			pipe_fds[0]);
		set_pipe_redir_out(to_exec, pipe_fds[1]);
		pipe_node = create_lst_node(pipe_fds);
		ft_lstadd_back(&ft_shell()->pipes, pipe_node);
		if (to_exec->next->is_in_par)
			set_pipe_redir_in_par(&to_exec->next, pipe_fds[0]);
		else
			set_pipe_redir_in(to_exec->next, pipe_fds[0]);
	}
	exec_child(to_exec);
	if (!ft_shell()->exec_in_par)
	{
		printf("closing pipes all pipes : %s\n",
			to_exec->token_node->contents[0]);
		close_pipes(to_exec);
	}
	if (ft_shell()->exec_in_par)
	{
		get_last_node_tree(ft_shell()->exec_tree, &last_node);
		if (to_exec->token_node == last_node->token_node)
		{
			// printf("closing pipes in node : %s\n",
			// to_exec->token_node->contents[0]);
			close_pipe_redir_in(to_exec); // WEIRD, SHOULD BE DELETED SOON
		}
	}
	return (0);
}

t_ast	*get_after_par_node(t_ast **lst)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		// if (current->token_node->contents)
		// 	printf("current after par searching = %s\n",
		// 		current->token_node->contents[0]);
		if (!current->is_in_par)
			return (current);
		current = current->next;
	}
	return (current);
}

t_ast	*ast_lst_dup(t_ast **lst)
{
	t_ast	*lst_cpy;
	t_ast	*current_cpy;
	t_ast	*current;

	current = *lst;
	lst_cpy = NULL;
	while (current)
	{
		current_cpy = malloc(sizeof(t_token));
		if (!current_cpy || gbg_coll(current_cpy, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		current_cpy->token_node = current->token_node;
		current_cpy->node_type = current->node_type;
		current_cpy->redirections = current->redirections;
		current_cpy->is_in_par = current->is_in_par;
		current_cpy->left = current->left;
		current_cpy->right = current->right;
		current_cpy->next = NULL;
		add_ast_lst(&lst_cpy, current_cpy);
		current = current->next;
	}
	return (lst_cpy);
}

t_ast	*ast_find_one(t_ast **lst, t_ast *node)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		if (current->token_node == node->token_node)
			return (current);
		current = current->next;
	}
	return (NULL);
}

// void    sub_tree_close_pipes(t_ast *root)
// {
//     if (root->left)
//         sub_tree_close_pipes(root->left);
//     close()
// }

// void	close_pipes_lst(t_ast **lst)
// {
// 	t_ast	*current;

// 	current = *lst;
// 	close_pipes(current->next->next->next->next);
// 	current = current->next;
// }

int	iterate_exec_ast_lst(t_ast **lst)
{
	t_ast	*current;
	t_ast	*par_sub_tree;
	int		cmd_nb;
	int		*after_par_pipe;
	t_ast	*last_par_node;
	t_ast	*par_lst;
	t_ast	*current_bis;
	t_ast	*last_node_tree;
	t_ast	*prev;
	int		pid;

	pid = -1;
	current = *lst;
	par_lst = NULL;
	cmd_nb = ast_list_size(lst);
	if (!cmd_nb)
		return (0);
	after_par_pipe = NULL;
	while (current)
	{
		printf("in loop cmd = %s\n", current->token_node->contents[0]);
		if (current->is_in_par && !ft_shell()->exec_in_par)
		{
			par_lst = ast_lst_dup(lst);
			printf("====== PAR LST ====\n");
			print_ast_lst(&par_lst);
			printf("========\n");
			current_bis = ast_find_one(&par_lst, current);
			par_sub_tree = find_top_node(&current_bis);
			get_last_node_tree(par_sub_tree, &last_par_node);
			if (pipe_after_par(last_par_node))
			{
				after_par_pipe = init_pipe();
				printf("pipe after par = %d %d\n", after_par_pipe[0],
					after_par_pipe[1]);
				set_pipe_redir_out_tree(par_sub_tree, after_par_pipe[1],
					"pipe");
				ft_lstadd_back(&ft_shell()->pipes,
					create_lst_node(after_par_pipe));
			}
			prev = current;
			current = get_after_par_node(&current);
			if (current)
				set_pipe_redir_in(current, after_par_pipe[0]);
            printf("next current = %s\n", current->token_node->contents[0]);
			// if (par_sub_tree->right->next)
			// 	printf("par sub tree right->next  = %s\n",
			// 		par_sub_tree->right->next->token_node->contents[0]);
			// printf("PAR SUB TREE ====\n");
			// print_tree(&par_sub_tree);
			// printf("=======\n");
			set_is_in_par(par_sub_tree, 0);
			set_next_null(par_sub_tree);
			printf("start exec PAR start\n");
			ft_shell()->exec_in_par = 1;
			pid = fork();
			if (pid == 0)
			{
                printf("forking main process\n");
                printf("START ===\n");
				ft_start_exec(&par_sub_tree);
				ft_shell()->exec_in_par = 0;
                printf("END   ===\n");
				exit(0);
			}
			// close(after_par_pipe[0]);
			if (after_par_pipe)
				close(after_par_pipe[1]);
			continue ;
		}
		prep_exec_child(current);
		prev = current;
		current = current->next;
	}
	if (after_par_pipe)
	{
		dprintf(2, "closing after pipe fd%d return (= %d\n", after_par_pipe[0],
			close(after_par_pipe[0]));
		dprintf(2, "closing after pipe fd%d return (= %d\n", after_par_pipe[1],
			close(after_par_pipe[1]));
		after_par_pipe = NULL;
	}
	get_last_node_tree(ft_shell()->full_exec_tree, &last_node_tree);
	dprintf(2, "last node tree = %s prev = %s\n",
		last_node_tree->token_node->contents[0], prev->token_node->contents[0]);
	if (prev->token_node == last_node_tree->token_node
		|| ft_shell()->exec_in_par)
	{
		dprintf(2, "closing all pipes\n");
		close_pipes_lst(&ft_shell()->pipes, NULL);
	}
	if (pid != -1)
		waitpid(pid, NULL, WUNTRACED);
	wait_all_pids(&ft_shell()->pids);
	// ft_shell()->pids_num = 0;
	ft_shell()->pids = NULL;
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
	ft_shell()->exec_tree = *tree;
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
