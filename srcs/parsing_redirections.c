/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redirections.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:13:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/17 18:10:26 by kipouliq         ###   ########.fr       */
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

t_redir *create_redir_node(t_token_type redir_type, char *filename)
{
    t_redir *redir_node;

    redir_node = malloc(sizeof(t_redir));
    if (!redir_node || gbg_coll(redir_node, PARSING, ADD))
        return (gbg_coll(redir_node, PARSING, ADD), exit(255), NULL);
    redir_node->redir_type = redir_type;
    redir_node->filename = filename;
    redir_node->next = NULL;
    return (redir_node);
}

t_redir *get_redir_lst_par(t_token **redir_node_lst)
{
    t_token *current;
    t_redir *new_redir_node;
    t_redir *redir_lst;

    current = *redir_node_lst;
    redir_lst = NULL;
    while (current && (is_a_redir_operator(current) || current->type == OUTFILE))
    {
        if (is_a_redir_operator(current) && current->next && current->next->type == OUTFILE)
        {
            printf("yo\n");
            new_redir_node = create_redir_node(current->type, current->next->content);
            printf("redir node type = %d filename = %s\n", current->type, current->next->content);
            add_redirection_node(&redir_lst, new_redir_node);
        }
        current = current->next;
    }
    return (redir_lst);
}

t_redir *find_last_redir_node(t_redir **lst)
{
    t_redir *current;

    current = *lst;
    if (!current)
        return (NULL);
    while (current->next)
        current = current->next;
    return (current);
}

t_redir	*redir_lst_dup(t_redir **lst)
{
	t_redir	*lst_cpy;
	t_redir	*current_cpy;
	t_redir	*current;

	current = *lst;
	lst_cpy = NULL;
	while (current)
	{
		current_cpy = malloc(sizeof(t_redir));
		if (!current_cpy || gbg_coll(current_cpy, PARSING, ADD))
			return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		current_cpy->filename = current->filename;
        current_cpy->redir_type = current->redir_type;
        current_cpy->next = NULL;
		add_redirection_node(&lst_cpy, current_cpy);
		current = current->next;
	}
	return (lst_cpy);
}

void    apply_redir_lst(t_token **lst, t_token *end_node, t_redir **redir_lst)
{
    t_token *current;
    t_redir *node_redir_save;

    current = *lst;
    while (current && current != end_node)
    {
        if (current->type == CMD)
        {
            printf("found cmd = %s\n", current->content);
            printf("===REDIR LST====\n");
            print_redir_lst(redir_lst);
            printf("================\n");
            node_redir_save = current->redirections;
            printf("===REDIR LST2====\n");
            print_redir_lst(redir_lst);
            printf("================\n");
            printf("===SAVE====\n");
            print_redir_lst(&node_redir_save);
            printf("===========\n");
            current->redirections = redir_lst_dup(redir_lst);
            printf("===REDIR LST3====\n");
            print_redir_lst(redir_lst);
            printf("================\n");
            find_last_redir_node(&current->redirections)->next = node_redir_save;
        }
        current = current->next;
    }
}

t_token *get_outfile_next_node(t_token **lst)
{
    t_token *current;

    current = *lst;
    while (current)
    {
        if (current->type == OUTFILE)
            return (current->next);
        current = current->next;
    }
    return (NULL);
}

int handle_par_redirection(t_token **lst, t_token *redir_node, t_token *closing_par)
{
    t_redir *redir_lst;
    t_token *current;
    t_token *par_right;

    current = *lst;
    par_right = NULL;
    while (current)
    {
        if (current->type == PAR_LEFT)
            par_right = find_closing_par(&current);
        if (par_right == closing_par)
        {
            redir_lst = get_redir_lst_par(&redir_node);
            apply_redir_lst(&current, closing_par, &redir_lst);
            closing_par->next = get_outfile_next_node(&closing_par);
            remove_token_node(lst, redir_node->next);
            remove_token_node(lst, redir_node);
            return (0);
        }
        current = current->next;
    }
    return (0);
}

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
        if (prev && prev->type == PAR_RIGHT && is_a_redir_operator(current))
        {
            printf("par redirection\n");
            handle_par_redirection(lst, current, prev);
            current = *lst;
            continue ;
        }
		else if (is_a_redir_operator(current))
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
