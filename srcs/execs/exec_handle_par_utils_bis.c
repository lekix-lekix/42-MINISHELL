/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_handle_par_utils_bis.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/20 16:05:15 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/24 15:03:13 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_back_redir(t_ast **lst, t_token **lst_dup)
{
	t_token	*curr;
	t_ast	*curr_match;

	curr = *lst_dup;
	while (curr)
	{
		if (curr->type == CMD)
		{
			curr_match = find_token_node(lst, curr);
			if (curr_match && curr_match->token_node->redirections)
			{
				curr->redirections = curr_match->token_node->redirections;
				curr->pipe_redir[0] = curr_match->token_node->pipe_redir[0];
				curr->pipe_redir[1] = curr_match->token_node->pipe_redir[1];
			}
		}
		curr = curr->next;
	}
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

t_token	*check_after_par_pipe(t_token **par_lst, int **after_par_pipe)
{
	t_token	*after_par_pipe_node;
	t_token	*after_par_lst;

	after_par_lst = NULL;
	after_par_pipe_node = NULL;
	after_par_pipe_node = pipe_after_par(par_lst);
	if (after_par_pipe_node)
	{
		after_par_lst = lst_dup(&after_par_pipe_node->next, NULL);
		*after_par_pipe = init_pipe();
		ft_lstadd_back(&ft_shell()->pipes,
			create_lst_node(&after_par_pipe[0][0]));
		ft_lstadd_back(&ft_shell()->pipes,
			create_lst_node(&after_par_pipe[0][1]));
	}
	return (after_par_lst);
}
