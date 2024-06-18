/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_extra_fts.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:23:18 by sabakar-          #+#    #+#             */
/*   Updated: 2024/06/18 14:57:11 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

char	*skip_spaces(char *str)
{
	int	i;

	i = 0;
	while (str[i] && ft_is_space(str[i]))
		i++;
	return (str + i);
}

void	clean_lst(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	if (!*lst)
		return ;
	current = *lst;
	if (!ft_strlen(current->content))
	{
		prev = current->next;
		gbg_coll(current->content, PARSING, FREE);
		gbg_coll(current, PARSING, FREE);
		*lst = prev;
	}
	current = *lst;
	while (current)
	{
		if (!ft_strlen(current->content))
		{
			prev->next = current->next;
			gbg_coll(current->content, PARSING, FREE);
			gbg_coll(current, PARSING, FREE);
		}
		prev = current;
		current = current->next;
	}
}

int	print_env(t_env **lst)
{
	t_env	*node;

	node = *lst;
	while (node)
	{
		printf("%s", node->field);
		printf("%s\n", node->content);
		node = node->next;
	}
	return (0);
}
