/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:15:37 by lekix             #+#    #+#             */
/*   Updated: 2024/08/11 17:03:09 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_pipe_stdout(int *after_par_pipe)
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
        printf("IM FORKING BITCH\n");
		pid = fork();
		if (pid == -1)
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
		if (pid == 0)
		{
			set_pipe_stdout(after_par_pipe);
			printf("EXECUTING PAR TREE ====\n");
			print_tree(&sub_tree);  
			printf("=====\n");
			ft_start_exec(&sub_tree);
			close_pipes_lst(&ft_shell()->pipes);
			printf("==== FORK PAR EXEC END ====\n");
			exit(0);
		}
		// close_pipes_lst(&ft_shell()->pipes);
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
		if (current->original_token == to_find->original_token)
			return (current);
		current = current->next;
	}
	return (NULL);
}

void	delete_begin_end_par_nodes(t_token **lst)
{
	t_token	*current;
	t_token	*end_par_node;

	current = *lst;
	if (current->type == PAR_LEFT)
	{
		end_par_node = find_closing_par(lst);
		*lst = current->next;
		current->next = NULL;
	}
	current = *lst;
	while (current && current->next)
	{
		if (current->next == end_par_node)
		{
			current->next = NULL;
			return ;
		}
		current = current->next;
	}
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

t_ast	*find_token_node(t_ast **lst, t_token *to_find)
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

t_token	*pipe_after_par(t_token **lst)
{
	t_token	*current;

	current = find_right_par(lst)->next;
	while (current)
	{
		if (current->type == PIPE)
			return (current);
		if (current->type == AND || current->type == OR)
			return (NULL);
		current = current->next;
	}
	return (NULL);
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

t_token	*find_left_par_original_token(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*par;

	current = *lst;
	par = NULL;
	while (current)
	{
		if (current->type == PAR_LEFT)
			par = current;
		if (current == node)
			return (par);
		current = current->next;
	}
	return (par);
}

void	set_back_redir(t_ast **lst, t_token **lst_dup)
{
	t_token	*current;
	t_ast	*current_match;

	current = *lst_dup;
	while (current)
	{
		if (current->type == CMD)
		{
			printf("current = %s original token = %s\n", current->contents[0],
				current->original_token->contents[0]);
			current_match = find_token_node(lst, current);
			if (current_match && current_match->token_node->redirections)
			{
				printf("match found = %s %s\n",
					current_match->token_node->contents[0],
					current_match->token_node->contents[1]);
				print_redir_lst(&current_match->token_node->redirections);
				current->redirections = current_match->token_node->redirections;
				current->pipe_redir[0] = current_match->token_node->pipe_redir[0];
				current->pipe_redir[1] = current_match->token_node->pipe_redir[1];
			}
		}
		current = current->next;
	}
}

void    set_pipe_redir_in_lst(t_token **lst, int *pipe_fds)
{
    t_token *current;
    t_redir *redir_out;

    current = *lst;
    while (current)
    {
        redir_out = create_redir_node(REDIR_INPUT, "pipe");
        add_front_redir_node(&current->redirections, redir_out);
        current->pipe_redir[0] = pipe_fds[0];
        current = current->next;
    }
}

int	handle_par_exec(t_ast **current)
{
	t_token	*par_lst;
	t_token	*after_par_lst;
	t_ast	*after_par_ast_lst;
	t_token	*original_current;
	t_token	*left_par;
	t_ast	*par_sub_tree;
	t_token	*after_par_pipe_node;
	int		*after_par_pipe;
	int		insert_node;
	int		par_exec_pid;

	// t_token	*next_node;
	printf("AST LST HANDLE PAR START\n");
	print_ast_lst(current);
	printf("====\n");
	after_par_pipe = NULL;
	after_par_lst = NULL;
	after_par_ast_lst = NULL;
	par_exec_pid = -1;
	printf("======== HANDLE PAR START ========\n");
	insert_node = 1;
	printf("ORIGINAL TOKEN LST ====\n");
	print_lst(&ft_shell()->les_token);
	printf("=======\n");
    printf("original current = %s %s\n", (*current)->token_node->original_token->contents[0], (*current)->token_node->original_token->contents[1]);
	original_current = find_original_token_lst(&ft_shell()->les_token,
			(*current)->token_node);
    printf("original current = %d\n", original_current->type);
	left_par = find_left_par_original_token(&ft_shell()->les_token,
			original_current);
	if (!left_par)
		printf("NO LEFT PAR\n");
	par_lst = lst_dup(&left_par, NULL);
	set_back_redir(current, &par_lst);
	printf("PAR LST BEFORE PAR DELETE ======\n");
	print_lst(&par_lst);
	printf("======\n");
	after_par_pipe_node = pipe_after_par(&par_lst);
	if (after_par_pipe_node)
	{
        printf("AFTER PIPE FOUND\n");
		after_par_lst = lst_dup(&after_par_pipe_node->next, NULL);
		after_par_pipe = init_pipe();
		ft_lstadd_back(&ft_shell()->pipes, create_lst_node(&after_par_pipe[0]));
		ft_lstadd_back(&ft_shell()->pipes, create_lst_node(&after_par_pipe[1]));
        // set_pipe_redir_in_lst(&after_par_lst, after_par_pipe);
	}
	delete_begin_end_par_nodes(&par_lst);
	printf("after deleting par nodes ======\n");
	print_lst(&par_lst);
	printf("======\n");
	set_par_lst(&par_lst);
	par_sub_tree = build_ast(&par_lst, &insert_node);
	printf("PAR SUB TREE ======\n");
	print_tree(&par_sub_tree);
	printf("===========\n");
	set_next_null(par_sub_tree);
	par_exec_pid = prep_exec_par(par_sub_tree, after_par_pipe);
	if (after_par_lst)
	{
		create_ast_exec_lst(&after_par_lst, &after_par_ast_lst);
		set_pipe_redir_in(after_par_ast_lst, after_par_pipe[0]);
		printf("AFTER PAR AST LST ====\n");
		print_ast_lst(&after_par_ast_lst);
		printf("=======\n");
        printf("after par pipe = %d %d\n", after_par_pipe[0], after_par_pipe[1]);
		iterate_exec_ast_lst(&after_par_ast_lst);
	}
	printf("===== HANDLE PAR FUNC END ====\n");
	return (par_exec_pid);
}
