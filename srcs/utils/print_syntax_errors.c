/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_syntax_errors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lekix <lekix@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:34:34 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/22 15:52:31 by lekix            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_token_syntax_error(t_token *node)
{
	char	*err;

	if (!node)
		return (print_newline_syntax_error(), -1);
	err = ft_join("minishell: syntax error near unexpected token `",
			node->content, PARSING);
	err = ft_join(err, "'\n", PARSING);
	write(2, err, ft_strlen(err));
	return (-1);
}

int	print_newline_syntax_error(void)
{
	write(2, "minishell: syntax error near unexpected token `newline'\n", 57);
	return (-1);
}

int	print_char_syntax_error(char *str)
{
	char	*err;

	err = ft_join("minishell: syntax error near unexpected token `", str,
			PARSING);
	err = ft_join(err, "'\n", PARSING);
	write(2, err, ft_strlen(err));
	return (-1);
}

int	print_ast_syntax_error(t_ast *node)
{
	char	*err;

	err = ft_join("minishell: syntax error near unexpected token `",
			node->token_node->content, PARSING);
	err = ft_join(err, "'\n", PARSING);
	write(2, err, ft_strlen(err));
	return (-1);
}
