/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:50:13 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/25 14:58:58 by sabakar-         ###   ########.fr       */
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
	if ((!ft_strlen(current->content) || only_spaces(current->content))
		&& !current->redirections)
	{
		prev = current->next;
		gbg_delete_node(current, PARSING);
		*lst = prev;
	}
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

t_token	*find_last_node(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return (NULL);
	while (current->next)
		current = current->next;
	return (current);
}

void	create_insert_split_nodes(t_token *prev, t_token *splt_node,
		char **contents)
{
	t_token	*new_lst;
	t_token	*new_node;
	int		i;

	i = 0;
	new_lst = NULL;
	while (contents[i])
	{
		new_node = create_cmd_node(contents[i], NULL);
		insert_node_lst(&new_lst, new_node);
		i++;
	}
	find_last_node(&new_lst)->next = splt_node->next;
	prev->next = new_lst;
}

void	split_lst_contents(t_token **lst)
{
	t_token	*current;
	t_token	*prev;
	char	**contents;

	contents = NULL;
	prev = NULL;
	current = *lst;
	if (!current)
		return ;
	while (current)
	{
		prev = current;
		if (current->content)
			contents = msh_split(current->content, ' ', PARSING);
		if (contents)
		{
			create_insert_split_nodes(prev, current, contents);
			remove_token_node(lst, current);
			current = *lst;
			continue ;
		}
		current = current->next;
	}
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
