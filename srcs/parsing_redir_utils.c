/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 17:48:46 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/28 17:51:55 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token *get_outfile_next_node(t_token **lst)
{
    t_token *current;

    current = *lst;
    while (current && (is_a_redir_operator(current) || current->type == OUTFILE))
        current = current->next;
    return (current);
}

void    remove_redir_nodes(t_token **lst, t_token **redir_lst)
{
    t_token *current;
    t_token *next;

    current = *redir_lst;
    while (current)
    {
        next = current->next;
        remove_token_node(lst, current);
        current = next;
    }
}

t_redir *find_last_redir_node(t_redir **lst)
{
    t_redir *current;

    current = *lst;
    if (!current)
        return (NULL);
    while (current->next)
        current = current->next;
    return (current);
}

t_redir	*redir_lst_dup(t_redir **lst)
{
	t_redir	*lst_cpy;
	t_redir	*current_cpy;
	t_redir	*current;

	current = *lst;
	lst_cpy = NULL;
	while (current)
	{
		current_cpy = malloc(sizeof(t_redir));
		if (!current_cpy || gbg_coll(current_cpy, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		current_cpy->filename = msh_strdup(current->filename, PARSING);
        current_cpy->redir_type = current->redir_type;
        current_cpy->next = NULL;
		add_redirection_node(&lst_cpy, current_cpy);
		current = current->next;
	}
	return (lst_cpy);
}

t_redir *create_redir_node(t_token_type redir_type, char *filename)
{
    t_redir *redir_node;

    redir_node = malloc(sizeof(t_redir));
    if (!redir_node || gbg_coll(redir_node, PARSING, ADD))
        return (gbg_coll(redir_node, PARSING, ADD), exit(255), NULL);
    redir_node->redir_type = redir_type;
    redir_node->filename = ft_strdup(filename);
    redir_node->next = NULL;
    return (redir_node);
}
