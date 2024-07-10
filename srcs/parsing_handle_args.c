/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_handle_args.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 14:50:38 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/10 12:33:23 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	get_nb_of_args(t_token **lst)
{
	t_token	*current;
	int		args_nb;

	current = *lst;
	args_nb = 0;
	while (current && !is_a_token_operator(current))
	{
		if (current->type == ARGS_FLAGS)
			args_nb++;
		current = current->next;
	}
	return (args_nb);
}

int	set_next_flags(t_token **lst, t_token *current, t_token *cmd_node)
{
	t_token	*next;
	int		i;

	i = 0;
	while (current && current->type == ARGS_FLAGS)
	{
		next = current->next;
		cmd_node->contents[++i] = msh_strdup(current->content, PARSING);
		remove_token_node(lst, current);
		current = next;
	}
	cmd_node->contents[i + 1] = NULL;
	*lst = current;
	return (0);
}

int	get_args_flags(t_token **lst)
{
	t_token	*current;
	t_token	*cmd_node;
	int		nb_of_args;

	cmd_node = *lst;
	current = (*lst)->next;
	nb_of_args = get_nb_of_args(lst);
	cmd_node->contents = malloc(sizeof(char *) * (nb_of_args + 2));
	if (!cmd_node->contents || gbg_coll(cmd_node->contents, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), -1);
	cmd_node->contents[0] = msh_strdup(cmd_node->content, PARSING);
	if (nb_of_args == 0)
	{
		cmd_node->contents[1] = NULL;
		*lst = current;
		return (0);
	}
	set_next_flags(lst, current, cmd_node);
	return (0);
}

void	join_cmd_args(t_token **lst)
{
	t_token	*current;
	// int		i;

	// i = 0;
	current = *lst;
	while (current)
	{
		if (current->type == CMD)
		{
			get_args_flags(&current);
			continue ;
		}
		current = current->next;
	}
}
