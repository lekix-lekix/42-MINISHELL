/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:15:37 by lekix             #+#    #+#             */
/*   Updated: 2024/08/07 19:18:10 by lekix            ###   ########.fr       */
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
		printf("EXECUTING PAR TREE ====\n");
		print_tree(&sub_tree);
		printf("=====\n");
		ft_start_exec(&sub_tree);
		close_pipes_lst(&ft_shell()->pipes);
		exit(0);
	}
	return (pid);
}

t_token	*find_original_token_lst(t_token **lst, t_token *to_find)
{
	t_token	*current;

	current = *lst;
	while (current)
	{
		if (current == to_find->original_token)
			return (current);
		current = current->next;
	}
	return (NULL);
}

t_token	*delete_last_node(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	while (current->next)
	{
		prev = current;
		current = current->next;
	}
	prev->next = NULL;
	return (prev);
}

void	set_par_node(t_token **lst, t_token *node)
{
	t_token	*current;
	int		par;

	current = *lst;
	par = 0;
	while (current)
	{
		if (current->type == PAR_LEFT)
			par++;
		if (current->type == PAR_RIGHT)
			par--;
		if (current == node)
		{
			if (!par)
				current->is_in_par = 0;
			else
				current->is_in_par = 1;
			return ;
		}
		current = current->next;
	}
}

void	set_par_lst(t_token **lst)
{
	t_token	*current;

	current = *lst;
	while (current)
	{
		set_par_node(lst, current);
		current = current->next;
	}
}

t_token	*cut_par_lst(t_token **lst)
{
	t_token	*current;
	int		flag;
	int		par;

	current = *lst;
	par = 0;
	flag = 0;
	while (current)
	{
		if (current->type == PAR_LEFT && flag == 0)
		{
			flag = 1;
			par++;
		}
		else if (current->type == PAR_LEFT)
			par++;
		if (current->type == PAR_RIGHT)
			par--;
		if (par == 0 && flag == 1 && current->next)
			return (current->next);
		current = current->next;
	}
	return (NULL);
}

t_ast	*find_original_token_ast_lst(t_ast **lst, t_token *to_find)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		if (current->token_node->original_token == to_find->original_token)
			return (current);
		current = current->next;
	}
	return (NULL);
}

int	handle_par_exec(t_ast **current, t_ast **exec_lst, int *before_par_pipe)
{
	t_token	*par_lst;
	t_token	*dup_par_lst;
	t_token	*next_node;
	t_ast	*par_sub_tree;
	// int		*after_par_pipe;
	int		insert_node;
	int		par_exec_pid;

    (void) exec_lst;
    (void) before_par_pipe;
    par_exec_pid = -1;
	printf("======== HANDLE PAR START ========\n");
	// t_ast   *last_node;
	// after_par_pipe = NULL;
	insert_node = 1;
	// par_lst = ast_lst_dup(current, get_after_par_node(current));
	// print_ast_lst(&par_lst);
	par_lst = find_original_token_lst(&ft_shell()->les_token,
			(*current)->token_node);
	if (!par_lst)
		printf("NO PAR LST\n");
	par_lst = lst_dup(&par_lst, NULL);
	set_par_lst(&par_lst);
	delete_last_node(&par_lst);
	// printf("lst ======\n");
	// print_lst(&par_lst);
	// printf("======\n");
	next_node = cut_par_lst(&par_lst);
	dup_par_lst = lst_dup(&par_lst, next_node);
	// printf("DUP PAR LST =====\n");
	set_par_lst(&par_lst);
	// print_lst(&dup_par_lst);
	// printf("=================\n");
	par_sub_tree = build_ast(&dup_par_lst, &insert_node);
    // printf("PAR SUB TREE ======\n");
    // print_tree(&par_sub_tree);
    // printf("===========\n");
	// par_sub_tree = find_top_node(&par_lst);
	// if (next_node)
    // {
        // *current = find_original_token_ast_lst(exec_lst, next_node->next);
        // (*current)->is_in_par = 0;
    // }
	// else
		// *current = NULL;
	// if (*current)
	// 	printf("AFTER PAR NODE = %s %s\n", (*current)->token_node->contents[0],
	// 		(*current)->token_node->contents[1]);
	// if (*current)
	// {
	// 	after_par_pipe = init_pipe();
	// 	ft_lstadd_back(&ft_shell()->pipes, create_lst_node(&after_par_pipe[0]));
	// 	ft_lstadd_back(&ft_shell()->pipes, create_lst_node(&after_par_pipe[1]));
	// }
	// if (*current && after_par_pipe)
	// 	set_pipe_redir_in(*current, after_par_pipe[0]);
	// set_is_in_par(par_sub_tree, 0);
	set_next_null(par_sub_tree);
    ft_start_exec(&par_sub_tree);
	// par_exec_pid = prep_exec_par(par_sub_tree, before_par_pipe, after_par_pipe);
	return (par_exec_pid);
}
