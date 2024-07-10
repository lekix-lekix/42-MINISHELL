/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:13:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/10 12:33:05 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	handle_redirection(t_token **lst, t_token **redir_node)
{
	t_token	*cmd_node;
	t_token	*filename_token;
	char	*filename;

	if (!(*redir_node)->next)
		return (-1);
	cmd_node = find_redir_node(lst, *redir_node);
	if (!cmd_node)
	{
		cmd_node = create_cmd_node(NULL, NULL);
		insert_node_lst(lst, cmd_node);
	}
	filename_token = (*redir_node)->next;
	filename = msh_strdup(filename_token->content, PARSING);
	add_redirection(cmd_node, *redir_node, filename);
	remove_token_node(lst, *redir_node);
	remove_token_node(lst, filename_token);
	*redir_node = *lst;
	return (0);
}

t_redir	*get_redir_lst_par(t_token **redir_node_lst/* , t_token *closing_par */)
{
	t_token	*current;
	t_redir	*new_redir_node;
	t_redir	*redir_lst;

	current = *redir_node_lst;
	redir_lst = NULL;
	while (current && (is_a_redir_operator(current)
			|| current->type == OUTFILE))
	{
		if (is_a_redir_operator(current) && current->next
			&& current->next->type == OUTFILE)
		{
			new_redir_node = create_redir_node(current->type,
					current->next->content);
			// closing_par = current->next->next;
			remove_token_node(redir_node_lst, current->next);
			remove_token_node(redir_node_lst, current);
			add_redirection_node(&redir_lst, new_redir_node);
			current = *redir_node_lst;
			continue ;
		}
		current = current->next;
	}
	return (redir_lst);
}

void	apply_redir_lst(t_token **lst, t_token *end_node, t_redir **redir_lst)
{
	t_token	*current;
	t_redir	*redir_save;

	current = *lst;
	while (current && current != end_node)
	{
		if (current->type == CMD)
		{
			redir_save = current->redirections;
			print_redir_lst(&redir_save);
			current->redirections = redir_lst_dup(redir_lst);
			find_last_redir_node(&current->redirections)->next = redir_save;
		}
		current = current->next;
	}
}

int	handle_par_redirection(t_token **lst, t_token **redir_node,
		t_token *closing_par)
{
	t_redir	*redir_lst;
	t_token	*current;
	t_token	*par_right;
	t_token	*next;

	current = *lst;
	par_right = NULL;
	while (current)
	{
		if (current->type == PAR_LEFT)
			par_right = find_closing_par(&current);
		if (par_right == closing_par)
		{
			next = get_outfile_next_node(&closing_par->next);
			redir_lst = get_redir_lst_par(redir_node/* , closing_par */);
			apply_redir_lst(&current, closing_par, &redir_lst);
			closing_par->next = next;
			return (0);
		}
		current = current->next;
	}
	*redir_node = *lst;
	return (0);
}

int	check_redirections(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	set_redir_lst(lst);
	set_args_lst(lst);
	prev = NULL;
	while (current)
	{
		if (is_a_redir_operator(current) && prev && prev->type == PAR_RIGHT)
		{
			if (handle_par_redirection(lst, &current, prev) == -1)
				return (-1);
			continue ;
		}
		else if (is_a_redir_operator(current))
		{
			if (handle_redirection(lst, &current) == -1)
				return (print_newline_syntax_error());
			continue ;
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
