/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:45:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/18 14:25:54 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	find_operator_type(char *input, t_token *node)
{
	int	op_len;

	check_operator_len(input, &op_len);
	if (*input == '(')
		node->type = PAR_LEFT;
	else if (*input == ')')
		node->type = PAR_RIGHT;
	else if (*input == '|' && op_len == 1)
		node->type = PIPE;
	else if (*input == '|' && op_len == 2)
		node->type = OR;
	else if (*input == '&' && op_len == 2)
		node->type = AND;
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
	find_operator_type(input_copy, node);
	node->next = NULL;
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
	node->next = NULL;
	return (node);
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

t_token	*tokenize_input(char *input)
{
	t_token	*root;
	t_token	*op_node;
	t_token	*cmd_node;
	char	*input_parse;
	char	*operator;

	root = NULL;
	input_parse = input;
	while (1)
	{
		operator = find_operator(input_parse);
		if (!operator)
		{
			(cmd_node = create_cmd_node(input_parse, NULL));
			insert_node_lst(&root, cmd_node);
			break ;
		}
		op_node = create_operator_node(&operator);
		if (!op_node)
			return (NULL);
		input_parse = skip_spaces(input_parse);
		cmd_node = create_cmd_node(input_parse, op_node->content);
		(insert_node_lst(&root, cmd_node), insert_node_lst(&root, op_node));
		input_parse = operator;
	}
	return (root);
}
