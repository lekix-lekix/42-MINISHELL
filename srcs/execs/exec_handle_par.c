/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:15:37 by lekix             #+#    #+#             */
/*   Updated: 2024/08/08 21:14:36 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_pipes_stdout(int *after_par_pipe)
{
	if (dup2(after_par_pipe[1], STDOUT_FILENO) == -1)
		return (perror("bash: dup2"), gbg_coll(NULL, ALL, FLUSH_ALL), exit(255),
			-1);
	close(after_par_pipe[1]);
	return (0);
}

int	prep_exec_par(t_ast *sub_tree, int *after_par_pipe)
{
	int	pid;

	if (after_par_pipe)
	{
		pid = fork();
		if (pid == -1)
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
		if (pid == 0)
		{
			printf("EXECUTING PAR TREE ====\n");
			print_tree(&sub_tree);
			printf("=====\n");
			ft_start_exec(&sub_tree);
			close_pipes_lst(&ft_shell()->pipes);
			exit(0);
		}
		return (pid);
	}
	ft_start_exec(&sub_tree);
	return (-1);
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
		if (par == 0 && flag == 1)
			return (current);
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

t_token	*find_last_nested_node_init_pipe(t_token **lst, int **pipe)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	prev = NULL;
	while (current)
	{
		if (current->type == PAR_LEFT)
			return (prev);
		if (current->type == PIPE)
		{
			*pipe = init_pipe();
			ft_lstadd_back(&ft_shell()->pipes,
				create_lst_node((void *)&pipe[0][0]));
			ft_lstadd_back(&ft_shell()->pipes,
				create_lst_node((void *)&pipe[0][1]));
		}
		prev = current;
		current = current->next;
	}
	return (prev);
}

int	handle_par_exec(t_ast **current)
{
	t_token	*par_lst;
	t_token	*next_node;
	t_ast	*par_sub_tree;
	int		*after_par_pipe;
	int		insert_node;
	int		par_exec_pid;

	after_par_pipe = NULL;
	par_exec_pid = -1;
	printf("======== HANDLE PAR START ========\n");
	insert_node = 1;
	par_lst = find_original_token_lst(&ft_shell()->les_token,
			(*current)->token_node);
	if (!par_lst)
		printf("NO PAR LST\n");
	par_lst = lst_dup(&par_lst, NULL);
	set_par_lst(&par_lst);
	// delete_last_node(&par_lst); // only if last node is a parenthesis
	printf("lst ======\n");
	print_lst(&par_lst);
	printf("======\n");
	next_node = cut_par_lst(&par_lst);
    printf("after find closing par lst ======\n");
	print_lst(&par_lst);
	printf("======\n");
	// next_node = find_last_nested_node_init_pipe(&next_node, &after_par_pipe); // de la merde
	if (next_node)
	{
		next_node->next = NULL;
		printf("next node = %d\n", next_node->type);
	}
	par_sub_tree = build_ast(&par_lst, &insert_node);
	if (after_par_pipe)
    {
        printf("looking for new current\n");
		*current = get_after_par_node(current);
    }
	printf("PAR SUB TREE ======\n");
	print_tree(&par_sub_tree);
	printf("===========\n");
	set_next_null(par_sub_tree);
	par_exec_pid = prep_exec_par(par_sub_tree, after_par_pipe);
	return (par_exec_pid);
}
