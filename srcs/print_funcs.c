/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:55:57 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/06 18:32:43 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_env(t_env **lst)
{
	t_env	*node;

	node = *lst;
	while (node)
	{
		printf("field: %s", node->field);
		printf("content: %s\n", node->content);
		node = node->next;
	}
	return (0);
}

int	print_real_env(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
		printf("%s\n", env[i]);
	return (0);
}

void	print_redir_lst(t_redir **lst)
{
	t_redir	*current;

	current = *lst;
	while (current)
	{
		printf("redir type = %d\n", current->redir_type);
		printf("redir filename = %s\n", current->filename);
		current = current->next;
	}
}

void	print_lst(t_token **lst)
{
	t_token	*root;

	root = *lst;
	while (root)
	{
		printf("--------\n");
		printf("content = '%s'\n", root->content);
		printf("type = %u\n", root->type);
        printf("in par = %d\n", root->is_in_par);
		if (root->redirections)
			print_redir_lst(&root->redirections);
		root = root->next;
	}
}
