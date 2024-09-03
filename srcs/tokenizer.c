/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:45:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/03 13:29:24 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	check_operator_len(char *str)
{
	int	i;

	i = 0;
	while (str[i] == str[0] && i < 2)
		i++;
	return (i);
}

t_token	*create_operator_node(char **input)
{
	t_token	*node;
	int		operator_len;
	char	*input_copy;

	input_copy = *input;
	operator_len = check_operator_len(input_copy);
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
	node->pipe_redir[0] = -1;
	node->pipe_redir[1] = -1;
	node->next = NULL;
	node->contents = NULL;
	node->original_token = NULL;
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
	ft_strcpy_sep_ptr(node->content, input, sep);
	node->redirections = NULL;
	node->pipe_redir[0] = -1;
	node->pipe_redir[1] = -1;
	node->contents = NULL;
	node->original_token = NULL;
	node->type = CMD;
	node->filename = NULL;
	node->next = NULL;
	return (node);
}

void	create_insert_token_nodes(t_token **lst, char **input, char **operator)
{
	t_token	*op_node;
	t_token	*cmd_node;

	op_node = create_operator_node(operator);
	cmd_node = create_cmd_node(*input, *operator);
	insert_node_lst(lst, cmd_node);
	insert_node_lst(lst, op_node);
	*input = *operator + ft_strlen(op_node->content);
}

char    *find_quote(char *input)
{
    char *str;
    int i;

    i = -1;
    while (input[++i])
    {
        if (input[i] == '\'' || input[i] == '\"')
        {
            str = ft_strchr(input + i + 1, input[i]);
            return (str);
        }
    }
    return (NULL);
}

t_token	*tokenize_input(char *input)
{
	t_token	*root;
	t_token	*cmd_node;
	// char	*tmp_input;
	char	*operator;

	root = NULL;
    // tmp_input = NULL;
	while (1)
	{
		operator = find_operator(input);
        // printf("operator")
		if (!operator)
		{
			cmd_node = create_cmd_node(input, NULL);
			insert_node_lst(&root, cmd_node);
			break ;
		}
		create_insert_token_nodes(&root, &input, &operator);
	}
	return (root);
}
