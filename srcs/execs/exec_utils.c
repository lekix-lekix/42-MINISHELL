/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/30 00:17:24 by sabakar-          #+#    #+#             */
/*   Updated: 2024/09/11 18:09:25 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

bool	ft_is_delimiter(char *delimiter, char *str)
{
	while (*str)
	{
		if (*delimiter == '"' || *delimiter == '\'')
		{
			delimiter++;
			continue ;
		}
		else if (*str == *delimiter)
		{
			str++;
			delimiter++;
		}
		else
			return (false);
	}
	while (*delimiter == '"' || *delimiter == '\'')
		delimiter++;
	return (!*delimiter);
}

static void	ft_del(void *ptr)
{
	free(ptr);
	ptr = NULL;
}

void	*ft_unset_cleaner(void *ptr, bool clean)
{
	static t_lst	*clean_list;

	if (clean)
	{
		ft_lstclear(&clean_list, ft_del);
		return (NULL);
	}
	else
	{
		ft_lstadd_back(&clean_list, ft_lstnew(ptr));
		return (ptr);
	}
}
