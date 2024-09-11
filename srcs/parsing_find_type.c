/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_find_type.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:47:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 20:48:10 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_an_operator(char c)
{
	return (c == '|' || c == '&' || c == '(' || c == ')' || c == '<'
		|| c == '>');
}

int	is_a_quote(char c)
{
	return (c == '\"' || c == '\'');
}

int	is_a_token_operator(t_token *node)
{
	return (node->type > ARGS_FLAGS && node->type < PAR_LEFT);
}

int	ft_is_space(char c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}
