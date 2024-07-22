/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_lst_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 11:50:13 by kipouliq          #+#    #+#             */
/*   Updated: 2024/07/12 16:58:24 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

t_token	*find_last_node(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return (NULL);
	while (current->next)
		current = current->next;
	return (current);
}

void	create_insert_split_nodes(t_token *prev, t_token *splt_node,
		char **contents)
{
	t_token	*new_lst;
	t_token	*new_node;
	int		i;

	i = 0;
	new_lst = NULL;
	while (contents[i])
	{
		new_node = create_cmd_node(contents[i], NULL);
		insert_node_lst(&new_lst, new_node);
		i++;
	}
	find_last_node(&new_lst)->next = splt_node->next;
	prev->next = new_lst;
}

void	split_lst_contents(t_token **lst)
{
	t_token	*current;
	t_token	*prev;
	char	**contents;

	contents = NULL;
	prev = NULL;
	current = *lst;
	if (!current)
		return ;
	while (current)
	{
		prev = current;
		if (current->content)
			contents = msh_split(current->content, ' ', PARSING);
		if (contents)
		{
			create_insert_split_nodes(prev, current, contents);
			remove_token_node(lst, current);
			current = *lst;
			continue ;
		}
		current = current->next;
	}
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

void	find_operator_type(char *input, t_token *node)
{
	int	op_len;

	op_len = check_operator_len(input);
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
	else
		node->type = CMD;
}
