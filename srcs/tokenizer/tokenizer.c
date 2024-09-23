/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:45:25 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/12 11:09:34 by sabakar-         ###   ########.fr       */
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

char	*find_quote(char *input)
{
	char	*str;
	int		i;

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

t_token	*create_cmd_node_no_sep(char *input)
{
	t_token	*node;

	node = malloc(sizeof(t_token));
	if (!node || gbg_coll(node, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	node->content = malloc(sizeof(char) * (ft_strlen(input) + 1));
	if (!node->content || gbg_coll(node->content, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), ft_close_fds(), exit(255),
			NULL);
	ft_strlcpy(node->content, input, ft_strlen(input) + 1);
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

void	more_tokenization(t_token **lst)
{
	t_token	*current;
	t_token	*new_cmd;
	t_token	*prev;
	char	*new_content;

	current = *lst;
	while (current)
	{
		if (content_count_words(current->content) > 1)
		{
			new_content = get_next_word(&current->content);
			new_cmd = create_cmd_node_no_sep(new_content);
			new_cmd->next = current;
			if (current == *lst)
				*lst = new_cmd;
			else
				prev->next = new_cmd;
			current = *lst;
			continue ;
		}
		prev = current;
		current = current->next;
	}
}

t_token	*tokenize_input(char *input)
{
	t_token	*root;
	t_token	*cmd_node;
	char	*operator;

	root = NULL;
	while (1)
	{
		operator = find_operator(input);
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
