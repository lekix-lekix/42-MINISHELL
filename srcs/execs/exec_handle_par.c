/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:15:37 by lekix             #+#    #+#             */
/*   Updated: 2024/08/01 17:23:35 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	prep_exec_par(t_ast *sub_tree, int *before_par_pipe, int *after_par_pipe)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	if (pid == 0)
	{
		if (before_par_pipe && dup2(before_par_pipe[0], STDIN_FILENO) == -1)
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
		if (after_par_pipe && dup2(after_par_pipe[1], STDOUT_FILENO) == -1)
			return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL),
				exit(255), -1);
		if (before_par_pipe)
			close(before_par_pipe[0]);
		if (after_par_pipe)
			close(after_par_pipe[1]);
		ft_start_exec(&sub_tree);
		close_pipes_lst(&ft_shell()->pipes);
		exit(0);
	}
	return (pid);
}

int	handle_par_exec(t_ast **current, int *before_par_pipe)
{
	t_ast	*par_lst;
	t_ast	*current_cpy;
	t_ast	*par_sub_tree;
	int		*after_par_pipe;
	int		par_exec_pid;

	after_par_pipe = NULL;
	par_lst = ast_lst_dup(current, get_after_par_node(current));
	current_cpy = ast_find_one(&par_lst, *current);
	par_sub_tree = find_top_node(&current_cpy);
	if (!par_sub_tree)
		current_cpy = *current;
	*current = get_after_par_node(current);
	if (*current)
	{
		after_par_pipe = init_pipe();
		ft_lstadd_back(&ft_shell()->pipes, create_lst_node(&after_par_pipe[0]));
		ft_lstadd_back(&ft_shell()->pipes, create_lst_node(&after_par_pipe[1]));
	}
	if (*current && after_par_pipe)
		set_pipe_redir_in(*current, after_par_pipe[0]);
	set_is_in_par(par_sub_tree, 0);
	set_next_null(par_sub_tree);
	par_exec_pid = prep_exec_par(par_sub_tree, before_par_pipe, after_par_pipe);
	return (par_exec_pid);
}
