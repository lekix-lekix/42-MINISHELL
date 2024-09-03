/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:13:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/03 12:02:39 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

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
    {
        printf("nb of words = %d\n", content_count_words(filename_token->content));
		filename_token->type = CMD;
    }
	filename = get_next_word(&filename_token->content);
    // printf("filename token after gnw = %s\n", filename_token->content);
	new_redir = create_redir_node((*redir_operator)->type, filename);
	redir_cmd = find_redir_node(lst, *redir_operator);
    // printf("redir cmd = %s\n", redir_cmd->content);
	if (!redir_cmd)
	{
		add_redirection_node(&filename_token->redirections, new_redir);
        gbg_coll(filename_token->content, PARSING, FREE);
        filename_token->content = malloc(sizeof(char));
        if (!filename_token || gbg_coll(filename_token->content, PARSING, ADD))
            return (gbg_coll(NULL, ALL, FLUSH_ALL));
        filename_token->content[0] = '\0';
        filename_token->type = CMD;
	}
	else
    {
		add_redirection_node(&redir_cmd->redirections, new_redir);
        remove_token_node(lst, filename_token);
    } 
	remove_token_node(lst, *redir_operator);
	*redir_operator = *lst;
	return (0);
}

t_redir	*get_redir_lst_par(t_token **redir_node_lst)
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
			new_redir_node = create_redir_node(REDIR_OUTPUT_APPEND,
					current->next->content);
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
