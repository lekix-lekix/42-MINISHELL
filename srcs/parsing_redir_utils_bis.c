/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir_utils_bis.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 17:56:28 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/01 15:19:24 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_redir_lst(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return ;
	while (current)
	{
		if (is_a_redir_operator(current) && current->next)
			current->next->type = OUTFILE;
		current = current->next;
	}
}

void	set_args_lst(t_token **lst)
{
	t_token	*current;
	int		found_cmd;

	current = *lst;
	if (!current)
		return ;
	found_cmd = 0;
	while (current)
	{
		if (current->type == CMD && found_cmd)
			current->type = ARGS_FLAGS;
		if (current->type == CMD)
		{
			found_cmd = 1;
			current = current->next;
			continue ;
		}
		if (is_a_token_operator(current))
			found_cmd = 0;
		current = current->next;
	}
}

void	add_redirection_node(t_redir **lst, t_redir *node)
{
	t_redir	*current;

	current = *lst;
	if (!current)
	{
		*lst = node;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = node;
}

int	add_redirection(t_token *cmd_node, t_token *redir_node, char *filename)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir || gbg_coll(new_redir, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	new_redir->redir_type = redir_node->type;
	new_redir->filename = filename;
	new_redir->next = NULL;
	add_redirection_node(&cmd_node->redirections, new_redir);
	return (0);
}
