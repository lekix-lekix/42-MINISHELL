/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_syntax_errors.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/28 16:34:34 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/28 16:35:43 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_token_syntax_error(t_token *node)
{
	printf("bash: syntax error near unexpected token `%s'\n", node->content);
	return (-1);
}

int	print_newline_syntax_error(void)
{
	printf("bash: syntax error near unexpected token `newline'\n");
	return (-1);
}

int	print_char_syntax_error(char *str)
{
	printf("bash: syntax error near unexpected token `%s'\n", str);
	return (-1);
}

int	print_ast_syntax_error(t_ast *node)
{
	printf("bash: syntax error near unexpected token `%s'\n",
		node->token_node->content);
	return (-1);
}
