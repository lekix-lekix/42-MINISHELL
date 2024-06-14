/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:13:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/14 15:21:40 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

void	set_redir_lst(t_token **lst)
{
	t_token	*current;

	current = *lst;
	if (!current)
		return ;
	while (current)
	{
		if (is_a_redir_operator(current) && current->next)
			current->next->type = OUTFILE;
		current = current->next;
	}
}

void	set_args_lst(t_token **lst)
{
	t_token	*current;
    int found_cmd;

	current = *lst;
	if (!current)
		return ;
    found_cmd = 0;
	while (current)
	{
        if (current->type == CMD && found_cmd)
            current->type = ARGS_FLAGS;
        if (current->type == CMD)
        {
            found_cmd = 1;
            current = current->next;
            continue ;
        }
        if (is_a_token_operator(current))
            found_cmd = 0;
		current = current->next;
	}
}

void	add_redirection_node(t_redir **lst, t_redir *node)
{
	t_redir	*current;

	current = *lst;
	if (!current)
	{
		*lst = node;
		return ;
	}
	while (current->next)
		current = current->next;
	current->next = node;
}

int	add_redirection(t_token *cmd_node, t_token *redir_node, char *filename)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (!new_redir || gbg_coll(new_redir, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
	new_redir->redir_type = redir_node->type;
	new_redir->filename = filename;
	new_redir->next = NULL;
	add_redirection_node(&cmd_node->redirections, new_redir);
	return (0);
}

char	*msh_strdup(const char *s, int mlc_lst)
{
	char	*final_str;
	int		s_len;

	s_len = ft_strlen(s);
	final_str = malloc(sizeof(char) * (s_len + 1));
	if (!final_str || gbg_coll(final_str, mlc_lst, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(final_str, s, s_len + 1);
	return (final_str);
}

int	handle_redirection(t_token **lst, t_token *redir_node)
{
	t_token	*cmd_node;
	t_token	*filename_token;
	char	*filename;

	if (!redir_node->next)
		return (-1); // syntax error : newline
	cmd_node = find_redir_node(lst, redir_node);
	if (!cmd_node)
	{
		cmd_node = create_cmd_node(NULL, NULL);
		insert_node_lst(lst, cmd_node);
	}
    filename_token = redir_node->next;
	filename = msh_strdup(filename_token->content, PARSING);
	add_redirection(cmd_node, redir_node, filename);
	remove_token_node(lst, redir_node);
	remove_token_node(lst, filename_token);
	return (0);
}

// int handle_par_redirection(t_token **lst, t_token *redir_node)
// {
    
// }

int	check_redirections(t_token **lst)
{
	t_token	*current;
    t_token *prev;

	current = *lst;
	if (!current)
		return (-1);
	set_redir_lst(lst);
    set_args_lst(lst);
    printf("LST AFTER SET REDIR LST =====\n");
    print_lst(lst);
    printf("LST END =====================\n");
    prev = NULL;
	while (current)
	{
        // if (prev && prev->type == PAR_RIGHT && is_a_redir_operator(current))
        // {
        //     handle_par_redirection(lst, current);
        //     current = *lst;
        //     continue ;
        // }
		if (is_a_redir_operator(current))
		{
			handle_redirection(lst, current);
			current = *lst;
            continue ;
		}
        prev = current;
		current = current->next;
	}
	return (0);
}
