/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check_syntax.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:27:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/23 17:06:32 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(char *str)
{
	int		i;
	char	c;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			c = str[i];
			str = ft_strchr(str + i + 1, c);
			if (!str)
			{
				printf("bash: syntax error near unexpected token `%c'\n", c);
				return (-1);
			}
			i = 0;
			continue ;
		}
	}
	return (0);
}

int	check_redir_syntax(t_token **input)
{
	t_token	*current;

	current = *input;
	while (current)
	{
		if (is_a_redir_operator(current) && (!current->next
				|| !current->next->content[0]))
			return (print_newline_syntax_error());
		if (is_a_redir_operator(current) && is_a_token_operator(current->next))
			return (print_token_syntax_error(current->next));
		if (is_a_redir_operator(current) && is_a_redir_operator(current->next))
			return (print_token_syntax_error(current->next));
		if (is_a_token_operator(current) && (!current->next
				|| !current->next->content[0]))
			return (print_token_syntax_error(current));
		if (is_a_token_operator(current) && is_a_token_operator(current->next))
			return (print_token_syntax_error(current));
		current = current->next;
	}
	return (0);
}
