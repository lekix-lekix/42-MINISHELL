/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_check_syntax.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:27:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/22 15:52:46 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_quotes(char *str)
{
	int		i;
	char	c;
	char	*err;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			c = str[i];
			str = ft_strchr(str + i + 1, c);
			if (!str)
			{
				err = ft_join("bash: syntax error near unexpected token `", &c,
						PARSING);
				err = ft_join(err, "'\n", PARSING);
				write(2, err, ft_strlen(err));
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
	t_token	*prev;

	prev = NULL;
	current = *input;
	while (current)
	{
		if (is_a_redir_operator(current) && (!current->next
				|| !current->next->content[0]))
			return (print_token_syntax_error(prev));
		if (is_a_redir_operator(current) && is_a_token_operator(current->next))
			return (print_token_syntax_error(prev));
		if (is_a_redir_operator(current) && is_a_redir_operator(current->next))
			return (print_token_syntax_error(current));
		if (is_a_token_operator(current) && (!current->next
				|| !current->next->content[0]))
			return (print_token_syntax_error(current));
		if (is_a_token_operator(current) && is_a_token_operator(current->next))
			return (print_token_syntax_error(current));
		prev = current;
		current = current->next;
	}
	return (0);
}
