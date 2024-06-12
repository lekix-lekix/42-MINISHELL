/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:45:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/12 16:57:51 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_operator_len(char *str, int *op_len)
{
	int	i;

	i = 0;
	while (str[i] == str[0])
		i++;
	// if (i > 2)==
	// 	return (0);
	*op_len = i;
	return (1);
}

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
	else if (*input == '<' && op_len == 1)
		node->type = REDIR_INPUT;
	else if (*input == '<' && op_len == 2)
		node->type = REDIR_HEREDOC;
	else if (*input == '>' && op_len == 1)
		node->type = REDIR_OUTPUT;
	else if (*input == '>' && op_len == 2)
		node->type = REDIR_OUTPUT_APPEND;
}

t_token	*create_operator_node(char **input)
{
	t_token	*node;
	int		operator_len;
	char	*input_copy;

	input_copy = *input;
	check_operator_len(input_copy, &operator_len);
	// if (!check_operator_len(input_copy, &operator_len))
	// 	return (printf("bash: syntax error near unexpected token `%c'\n",
	// 			*input_copy), NULL);
    printf("operator len = %d\n", operator_len);
	node = malloc(sizeof(t_token));
	if (!node || gbg_coll(node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	find_operator_type(input_copy, node);
	if ((node->type == PAR_LEFT || node->type == PAR_RIGHT) && operator_len > 1)
		operator_len = 1;
	node->content = malloc(sizeof(char) * (operator_len + 1));
	if (!node->content || gbg_coll(node->content, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(node->content, input_copy, operator_len + 1);
	node->filename = NULL;
	node->redirections = NULL;
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
	node->redirections = NULL;
	node->type = CMD;
	node->filename = NULL;
	node->next = NULL;
	return (node);
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
		operator= find_operator(input_parse);
		if (!operator)
		{
            // printf("input parse = %s\n", input_parse);
			cmd_node = create_cmd_node(input_parse, NULL);
			insert_node_lst(&root, cmd_node);
			break ;
		}
		op_node = create_operator_node(&operator);
		if (!op_node)
			return (NULL);
		input_parse = skip_spaces(input_parse);
		cmd_node = create_cmd_node(input_parse, op_node->content);
		insert_node_lst(&root, cmd_node);
		insert_node_lst(&root, op_node);
		input_parse = operator;
	}
	return (root);
}
