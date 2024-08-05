/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_close_pipes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 16:11:49 by lekix             #+#    #+#             */
/*   Updated: 2024/08/02 17:40:26 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	close_pipe_redir_in(t_ast *node)
{
	t_redir	*redirection;

	redirection = node->token_node->redirections;
	if (!redirection)
		return (-1);
	while (redirection)
	{
		if (redirection->redir_type == REDIR_INPUT
			&& ft_strcmp(redirection->filename, "pipe") == 0)
			close(node->token_node->pipe_redir[0]);
		redirection = redirection->next;
	}
	return (0);
}

void	close_pipe_redir_out(t_ast *node)
{
	t_redir	*redirection;

	redirection = node->token_node->redirections;
	if (!redirection)
		return ;
	while (redirection)
	{
		if (redirection->redir_type == REDIR_OUTPUT
			&& ft_strcmp(redirection->filename, "pipe") == 0)
			close(node->token_node->pipe_redir[1]);
		redirection = redirection->next;
	}
}

void	set_pipe_redir_in_par(t_ast **in_par_lst, int pipe_fd)
{
	t_ast	*current;

	current = *in_par_lst;
	while (current && current->is_in_par)
	{
		set_pipe_redir_in(current, pipe_fd);
		current = current->next;
	}
}

void	set_pipe_redir_out(t_ast *node, int pipe_fd)
{
	t_redir	*output_redir;

	output_redir = create_redir_node(REDIR_OUTPUT, "pipe");
	add_front_redir_node(&node->token_node->redirections, output_redir);
	node->token_node->pipe_redir[1] = pipe_fd;
}

void	set_pipe_redir_in(t_ast *node, int pipe_fd)
{
	t_redir	*input_redir;

	input_redir = create_redir_node(REDIR_INPUT, "pipe");
	add_front_redir_node(&node->token_node->redirections, input_redir);
	node->token_node->pipe_redir[0] = pipe_fd;
}
 