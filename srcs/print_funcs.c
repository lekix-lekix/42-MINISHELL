/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_funcs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/01 13:55:57 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/27 14:45:26 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	print_env(t_env **lst)
{
	t_env	*node;

	node = *lst;
	while (node)
	{
		printf("%s", node->field);
		printf("%s\n", node->content);
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
	int		i;

	root = *lst;
	while (root)
	{
		i = 0;
		printf("--------\n");
		if (root->content)
			printf("content = '%s'\n", root->content);
		if (root->contents)
		{
			while (root->contents[i])
			{
				printf("contents[%d] = %s\n", i, root->contents[i]);
				i++;
			}
		}
		printf("type = %u\n", root->type);
		if (root->redirections)
			print_redir_lst(&root->redirections);
		root = root->next;
	}
}

void	print_ast_lst(t_ast **lst)
{
	t_ast	*current;

	current = *lst;
	while (current)
	{
		printf("--------\n");
		printf("content = '%s'", current->token_node->contents[0]);
		if (current->node_type == CMD)
			printf(" %s\n", current->token_node->contents[0]);
		if (current->token_node->redirections)
			print_redir_lst(&current->token_node->redirections);
		current = current->next;
	}
}
