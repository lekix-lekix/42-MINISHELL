/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_clean_tokens.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:22:21 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/27 15:04:07 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	check_clean_lst(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	while (current)
	{
		if ((!ft_strlen(current->content) || only_spaces(current->content))
			&& !current->redirections)
		{
			prev->next = current->next;
			gbg_delete_node(current, PARSING);
			current = *lst;
			continue ;
		}
		prev = current;
		current = current->next;
	}
}

void	clean_lst(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	if (!*lst)
		return ;
	current = *lst;
	if ((!ft_strlen(current->content) || only_spaces(current->content))
		&& !current->redirections)
	{
		prev = current->next;
		gbg_delete_node(current, PARSING);
		*lst = prev;
	}
	check_clean_lst(lst);
}

void	remove_token_node(t_token **lst, t_token *node)
{
	t_token	*current;
	t_token	*prev;

	if (!*lst)
		return ;
	current = *lst;
	if (current == node)
	{
		prev = current->next;
		gbg_delete_node(current, PARSING);
		*lst = prev;
		return ;
	}
	current = *lst;
	while (current)
	{
		if (current == node)
		{
			prev->next = current->next;
			gbg_delete_node(current, PARSING);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

int	clean_token_lst(t_token **lst)
{
	clean_lst(lst);
	trim_token_fields(lst);
	return (0);
}

int	trim_token_fields(t_token **lst)
{
	t_token	*current;
	char	*str;

	current = *lst;
	while (current)
	{
		if (current->content && ft_strlen(current->content))
		{
			str = msh_strtrim(current->content, " ");
			gbg_coll(current->content, PARSING, FREE);
			current->content = str;
		}
		if (current->filename && ft_strlen(current->content))
		{
			str = msh_strtrim(current->filename, " ");
			gbg_coll(current->filename, PARSING, FREE);
			current->filename = str;
		}
		current = current->next;
	}
	return (0);
}
