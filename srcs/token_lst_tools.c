/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:50:13 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/10 17:30:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	clean_lst(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	if (!*lst)
		return ;
	current = *lst;
	if (!ft_strlen(current->content) || only_spaces(current->content))
	{
		prev = current->next;
		gbg_delete_node(current, PARSING);
		*lst = prev;
	}
	current = *lst;
	while (current)
	{
		if (!ft_strlen(current->content) || only_spaces(current->content))
		{
			prev->next = current->next;
			gbg_delete_node(current, PARSING);
		}
		prev = current;
		current = current->next;
	}
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
		}
		prev = current;
		current = current->next;
	}
}

// int check_if_trim(char *str)
// {
//     if (ft_strlen(str) == 0)
//         return (0);
//     if (ft_is_space(str[0]) || ft_is_space(str[ft_strlen(str) - 1]))
//         return (1);
//     return (0);
// }

int	trim_token_fields(t_token **lst)
{
	t_token	*current;
	char	*str;

	current = *lst;
	while (current)
	{
		if (current->content)
		{
			str = msh_strtrim(current->content, " ");
            printf("str = %s\n", str);
			gbg_coll(current->content, PARSING, FREE);
			current->content = str;
		}
		if (current->filename)
		{
			str = msh_strtrim(current->filename, " ");
            printf("str = %s\n", str);
			gbg_coll(current->filename, PARSING, FREE);
			current->filename = str;
		}
		current = current->next;
	}
	return (0);
}

int	clean_token_lst(t_token **lst)
{
	clean_lst(lst);
	trim_token_fields(lst);
	return (0);
}

void	insert_node_lst(t_token **lst, t_token *node)
{
	t_token	*root;

	root = *lst;
	if (!root)
	{
		*lst = node;
		return ;
	}
	while (root->next)
		root = root->next;
	root->next = node;
}
