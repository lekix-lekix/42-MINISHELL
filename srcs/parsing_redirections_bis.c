/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections_bis.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:40:44 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/07 17:41:18 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_filename(t_token *node)
{
	int		i;
	char	*filename;
	int		filename_len;

	i = 0;
	filename_len = ft_strlen_sep(node->content, " ");
	filename = malloc(sizeof(char) * (filename_len + 1));
	if (!filename || gbg_coll(filename, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(filename, node->content, filename_len);
	i = -1;
	while (node->content[++i] && i < filename_len)
		node->content[i] = ' ';
	if (!only_spaces(node->content))
		node->type = CMD;
	return (filename);
}

t_token	*find_redir_node(t_token **lst, t_token *redir_node)
{
	t_token	*cmd_node;
	t_token	*current;
	int		found_redir_node;

	current = *lst;
	if (!current)
		return (NULL);
	found_redir_node = 0;
	cmd_node = NULL;
	while (current)
	{
		if (current == redir_node)
			found_redir_node = 1;
		if (current->type == CMD)
			cmd_node = current;
		if ((current->type > CMD && current->type < REDIR_INPUT)
			&& found_redir_node)
			return (cmd_node);
		current = current->next;
	}
	return (cmd_node);
}
