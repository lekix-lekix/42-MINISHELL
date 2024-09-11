/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:15:37 by lekix             #+#    #+#             */
/*   Updated: 2024/09/11 17:59:24 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_pipe_stdout(int *after_par_pipe)
{
	if (dup2(after_par_pipe[1], STDOUT_FILENO) == -1)
		return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
			ft_close_fds(), exit(255), -1);
	close(after_par_pipe[1]);
	ft_close_fds();
	return (0);
}

int	prep_exec_par(t_ast *sub_tree, int *after_par_pipe)
{
	int	pid;

	if (after_par_pipe)
	{
		pid = fork();
		if (pid == -1)
			return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
				-1);
		if (pid == 0)
		{
			(ft_shell())->signint_child = true;
			set_pipe_stdout(after_par_pipe);
			ft_start_exec(&sub_tree);
			close_pipes_lst(&ft_shell()->pipes);
			exit(0);
		}
		return (pid);
	}
	ft_start_exec(&sub_tree);
	return (-1);
}

void	create_ast_exec_lst(t_token **lst, t_ast **exec_lst)
{
	t_token	*current;
	t_ast	*node;

	current = *lst;
	while (current && current->type != AND && current->type != OR)
	{
		if (current->type == CMD)
		{
			node = create_ast_node(current);
			add_ast_lst(exec_lst, node);
		}
		current = current->next;
	}
}

int	build_run_sub_exec(t_token **par_lst)
{
	t_token	*after_par_lst;
	t_ast	*par_sub_tree;
	t_ast	*after_par_ast_lst;
	int		*after_par_pipe;
	int		par_exec_pid;

	after_par_pipe = NULL;
	after_par_ast_lst = NULL;
	par_exec_pid = -1;
	after_par_lst = check_after_par_pipe(par_lst, &after_par_pipe);
	delete_begin_end_par_nodes(par_lst);
	set_par_lst(par_lst);
	par_sub_tree = build_ast(par_lst, NULL);
	set_next_null(par_sub_tree);
	par_exec_pid = prep_exec_par(par_sub_tree, after_par_pipe);
	if (after_par_lst)
	{
		create_ast_exec_lst(&after_par_lst, &after_par_ast_lst);
		set_pipe_redir_in(after_par_ast_lst, after_par_pipe[0]);
		iterate_exec_ast_lst(&after_par_ast_lst);
	}
	return (par_exec_pid);
}

int	handle_par_exec(t_ast **current)
{
	t_token	*par_lst;
	t_token	*left_par;

	left_par = find_left_par_original_token(&ft_shell()->les_token,
			find_original_token_lst(&ft_shell()->les_token,
				(*current)->token_node));
	par_lst = lst_dup(&left_par, NULL);
	set_back_redir(current, &par_lst);
	return (build_run_sub_exec(&par_lst));
}
