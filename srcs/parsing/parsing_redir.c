/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:13:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 19:45:31 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	add_empty_redir_node(t_token *filename_token, t_redir *new_redir)
{
	add_redirection_node(&filename_token->redirections, new_redir);
	gbg_coll(filename_token->content, PARSING, FREE);
	filename_token->content = malloc(sizeof(char));
	if (!filename_token || gbg_coll(filename_token->content, PARSING, ADD))
		return (ft_exit_close(255), -1);
	filename_token->content[0] = '\0';
	filename_token->type = CMD;
	return (0);
}

int	handle_redirection(t_token **lst, t_token **redir_operator)
{
	t_token	*filename_token;
	char	*filename;
	t_token	*redir_cmd;
	t_redir	*new_redir;

	if (!(*redir_operator)->next)
		return (-1);
	filename_token = (*redir_operator)->next;
	if (content_count_words(filename_token->content) > 1)
		filename_token->type = CMD;
	filename = get_next_word(&filename_token->content);
	new_redir = create_redir_node((*redir_operator)->type, filename);
	redir_cmd = find_redir_node(lst, *redir_operator);
	if (!redir_cmd)
		add_empty_redir_node(filename_token, new_redir);
	else
	{
		add_redirection_node(&redir_cmd->redirections, new_redir);
		remove_token_node(lst, filename_token);
	}
	remove_token_node(lst, *redir_operator);
	*redir_operator = *lst;
	return (0);
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
			redir_lst = get_redir_lst_par(redir_node);
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
				return (print_token_syntax_error(prev));
			continue ;
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
