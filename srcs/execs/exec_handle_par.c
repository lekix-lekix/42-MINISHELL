/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:15:37 by lekix             #+#    #+#             */
/*   Updated: 2024/09/24 18:19:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	set_pipe_stdout(int *after_par_pipe)
{
	if (dup2(after_par_pipe[1], STDOUT_FILENO) == -1)
		return (ft_exit_close(255), -1);
	close(after_par_pipe[1]);
	ft_close_fds();
	return (0);
}

int	prep_exec_par(t_ast *sub_tree, int *after_par_pipe)
{
	int	pid;
	int	status;

	if (after_par_pipe)
	{
		pid = fork();
		if (pid == -1)
			return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_exit_close(255), -1);
		if (pid == 0)
		{
			dprintf(2, "FORK STARTED ====\n");
			(ft_shell())->signint_child = true;
			set_pipe_stdout(after_par_pipe);
			ft_start_exec(&sub_tree);
			close_pipes_lst(&ft_shell()->pipes);
			ft_exit_close(0);
		}
		waitpid(pid, &status, WUNTRACED);
		if (WIFEXITED(status))
			ft_shell()->exit_status = WEXITSTATUS(status);
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

void	print_redir_lst(t_redir **lst, t_token *node)
{
	t_redir	*current;

	current = *lst;
	printf("====== REDIRS ======\n");
	while (current)
	{
		dprintf(2, "node %s %s redir type = %d\n", node->contents[0],
			node->contents[1], current->redir_type);
		if (current->filename)
			printf("filename = %s\n", current->filename);
		current = current->next;
	}
}

void	print_tree(t_ast *tree)
{
	if (tree->left)
		print_tree(tree->left);
	if (tree->token_node->type == CMD)
	{
		printf("tree node = %s %s\n", tree->token_node->contents[0],
			tree->token_node->contents[1]);
		if (tree->redirections)
			print_redir_lst(&tree->token_node->redirections, tree->token_node);
	}
	else
		printf("operator node type = %d\n", tree->node_type);
	if (tree->right)
		print_tree(tree->right);
}

void	print_ast_lst(t_ast **lst)
{
	t_ast	*curr;

	curr = *lst;
	while (curr)
	{
		if (curr->node_type == CMD)
			printf("after par ast curr = %s %s\n",
				curr->token_node->contents[0], curr->token_node->contents[1]);
		else
			printf("after par operator node, type = %d\n", curr->node_type);
		curr = curr->next;
	}
}

// int	build_run_sub_exec(t_token **par_lst, t_token *left_par)
// {
// 	t_token	*after_par_lst;
// 	t_ast	*par_sub_tree;
// 	t_ast	*after_par_ast_lst;
// 	int		*after_par_pipe;
// 	int		par_exec_pid;

// 	after_par_pipe = NULL;
// 	after_par_ast_lst = NULL;
// 	par_exec_pid = -1;
// after_par_lst = check_after_par_pipe(, &after_par_pipe);
// 	delete_begin_end_par_nodes(par_lst);
// 	set_par_lst(par_lst);
// 	par_sub_tree = build_ast(par_lst, NULL);
// 	print_tree(par_sub_tree);
// 	set_next_null(par_sub_tree);
// 	dprintf(2, "EXEC PAR START======\n");
// 	par_exec_pid = prep_exec_par(par_sub_tree, after_par_pipe);
// 	// if (after_par_lst)
// 	// {
// 	// 	create_ast_exec_lst(&after_par_lst, &after_par_ast_lst);
// 	// 	print_ast_lst(&after_par_ast_lst);
// 	// 	set_pipe_redir_in(after_par_ast_lst, after_par_pipe[0]);
// 	// 	iterate_exec_ast_lst(&after_par_ast_lst);
// 	// }
// 	return (par_exec_pid);
// }

t_token	*get_last_token_node(t_token **lst)
{
	t_token	*current;

	current = *lst;
	while (current->next)
		current = current->next;
	return (current);
}

int	build_run_sub_exec(t_token **par_lst)
{
	t_ast	*par_sub_tree;
	t_token	*after_par_lst;
	t_ast	*after_par_ast_lst;
	t_token	*last_node;
	int		*after_par_pipe;
	int		par_exec_pid;

	// t_ast	*curr;
	par_exec_pid = -1;
	last_node = find_original_token_lst(&ft_shell()->les_token,
			get_last_token_node(par_lst));
	after_par_lst = check_after_par_pipe(&last_node, &after_par_pipe);
	delete_begin_end_par_nodes(par_lst);
	set_par_lst(par_lst);
	par_sub_tree = build_ast(par_lst, NULL);
	print_tree(par_sub_tree);
	set_next_null(par_sub_tree);
	prep_exec_par(par_sub_tree, NULL);
	if (after_par_lst)
	{
		dprintf(2, "YES AFTER PAR\n");
		create_ast_exec_lst(&after_par_lst, &after_par_ast_lst);
		print_ast_lst(&after_par_ast_lst);
		set_pipe_redir_in(after_par_ast_lst, after_par_pipe[0]);
		iterate_exec_ast_lst(&after_par_ast_lst);
	}
	return (par_exec_pid);
}

t_ast	*get_next_ast_node(t_ast **lst, t_token *to_find)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		if (current->token_node == to_find)
			return (current->next);
		current = current->next;
	}
	dprintf(2, "didnt find next\n");
	return (NULL);
}

t_token	*convert_cpy_token_ast_lst(t_ast **lst)
{
	t_ast	*current;
	t_token	*current_cpy;
	t_token	*original;
	t_token	*final_lst;

	current = *lst;
	final_lst = NULL;
	printf("convert copy ====\n");
	while (current)
	{
		if (current->node_type == CMD)
			printf("current = %s %s\n", current->token_node->contents[0],
				current->token_node->contents[1]);
		else
			printf("current op node = %d\n", current->node_type);
		current_cpy = malloc(sizeof(t_token));
		if (!current_cpy || gbg_coll(current_cpy, PARSING, ADD))
			return (ft_exit_close(255), NULL);
		original = find_original_token_lst(&ft_shell()->les_token,
				current->token_node);
		if (!original)
		{
			if (current->node_type == CMD)
				printf("current no original = %s %s\n",
					current->token_node->contents[0],
					current->token_node->contents[1]);
			else
				printf("current op node no original= %d\n", current->node_type);
            printf("token node = %s\n", current->token_node->contents[0]);
		}
		copy_token_attributes(original, current_cpy);
		insert_node_lst(&final_lst, current_cpy);
		current = current->next;
	}
	return (final_lst);
}

int	handle_par_exec(t_ast **current)
{
	t_token	*par_lst;
	t_token	*curr;
	t_ast	*curr1;

	// t_token	*left_par;
	// t_token	*original_token;
	// left_par = find_left_par_original_token(&ft_shell()->les_token,
	// 		find_original_token_lst(&ft_shell()->les_token,
	// 			(*current)->token_node));
	// original_token = find_original_token_lst(&ft_shell()->les_token,
	// 		(*current)->token_node);
	// dprintf(2, "left par next = %d\n", left_par->next->type);
	// right_par = find_closing_par(&left_par);
	// if (right_par)
	// 	dprintf(2, "right par = %d\n", right_par->type);
	// par_lst = lst_dup(&left_par, right_par);
	dprintf(2, "handle par ============\n");
	curr1 = *current;
	while (curr1)
	{
		if (curr1->node_type == CMD)
			printf("par lst curr = %s %s\n", curr1->token_node->contents[0],
				curr1->token_node->contents[1]);
		else
			printf("par lst operator node type = %d\n", curr1->node_type);
		curr1 = curr1->next;
	}
	par_lst = convert_cpy_token_ast_lst(current);
	curr = par_lst;
	while (curr)
	{
		if (curr->type == CMD)
			printf("par lst curr = %s %s\n", curr->contents[0],
				curr->contents[1]);
		else
			printf("par lst operator node type = %d\n", curr->type);
		curr = curr->next;
	}
	set_back_redir(current, &par_lst);
	// printf("*current in ast lst next = %s\n",
	// (*current)->next->token_node->contents[0]);
	// *current = get_next_ast_node(current, right_par);
	return (build_run_sub_exec(&par_lst));
}
