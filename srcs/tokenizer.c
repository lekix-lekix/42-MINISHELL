/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:45:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/16 18:34:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_operator_len(char *str, int *op_len)
{
	int	i;

	i = 0;
	while (str[i] == str[0])
		i++;
	if (i > 2)
		return (0);
	*op_len = i;
	return (1);
}

t_token	*create_operator_node(char **input)
{
	t_token	*node;
	int		operator_len;
	char	*input_copy;

	input_copy = *input;
	if (!check_operator_len(input_copy, &operator_len))
		return (printf("bash: syntax error near unexpected token `%c'\n",
				*input_copy), NULL);
	node = malloc(sizeof(t_token));
	if (!node || gbg_coll(node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	node->content = malloc(sizeof(char) * (operator_len + 1));
	if (!node->content || gbg_coll(node->content, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(node->content, input_copy, operator_len + 1);
	node->type = OPERATOR;
	node->left = NULL;
	node->right = NULL;
	*input += operator_len;
	return (node);
}

t_token	*create_cmd_node(char *input, char *sep)
{
	t_token	*node;
	int		cmd_len;

	cmd_len = ft_strlen_sep(input, sep);
	node = malloc(sizeof(t_token));
	if (!node || gbg_coll(node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	node->content = malloc(sizeof(char) * (cmd_len + 1));
	if (!node->content || gbg_coll(node->content, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strcpy_sep(node->content, input, sep);
	node->type = CMD;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_token	*tokenize_input(char *input)
{
	t_token	*root;
	t_token	*cmd_node;
	t_token	*op_node;
	char	*input_parse;
	char	*operator;

	root = NULL;
	input_parse = input;
	while (input_parse)
	{
		operator = find_operator(input_parse);
		if (!operator)
		{
			cmd_node = create_cmd_node(input_parse, NULL);
			insert_cmd_node(&root, cmd_node);
			break ;
		}
		op_node = create_operator_node(&operator);
		if (!op_node)
			return (NULL);
		cmd_node = create_cmd_node(input_parse, op_node->content);
		insert_operator_node(&root, op_node);
		insert_cmd_node(&root, cmd_node);
		input_parse = operator;
	}
	return (root);
}
