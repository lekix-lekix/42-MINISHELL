/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 17:13:58 by kipouliq          #+#    #+#             */
/*   Updated: 2024/08/28 18:48:37 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	**new_arr_index_minus_one(char **arr)
{
	char	**new_arr;
	int		arr_size;
	int		i;

	arr_size = get_arr_len(arr);
	new_arr = malloc(sizeof(char *) * get_arr_len(arr));
	if (!new_arr || gbg_coll(new_arr, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	i = -1;
	while (++i < arr_size - 1)
		new_arr[i] = arr[i + 1];
	new_arr[i] = NULL;
	return (new_arr);
}

// int	handle_redirection(t_token **lst, t_token **redir_operator)
// {
// 	t_redir	*new_redir;
// 	t_token	*filename_token;
// 	t_token	*cmd_node;
// 	char	*filename;

// 	if (!(*redir_operator)->next)
// 		return (-1);
//     printf("uo\n");
// 	filename_token = (*redir_operator)->next;
// 	filename = msh_strdup(filename_token->contents[0], PARSING);
//     printf("filename = %s\n", filename);
// 	if (get_arr_len(filename_token->contents) > 1)
//     {
// 		filename_token->contents = new_arr_index_minus_one(filename_token->contents);
//         filename_token->type = CMD;
//     }
// 	new_redir = create_redir_node((*redir_operator)->type, filename);
// 	if (get_arr_len(filename_token->contents) == 1)
// 	{
// 		cmd_node = create_cmd_node(NULL, NULL);
//         cmd_node->contents = malloc(sizeof(char *));
//         if (!cmd_node->contents || gbg_coll(cmd_node->contents, PARSING,
// ADD))
//             return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
//         cmd_node->contents[0] = malloc(sizeof(char));
//         if (!cmd_node->contents[0] || gbg_coll(cmd_node->contents[0],
// PARSING, ADD))
//             return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
//         cmd_node->contents[0][0] = '\0';
// 		insert_node_lst(lst, cmd_node);
// 		add_redirection_node(&cmd_node->redirections, new_redir);
// 	}
// 	else
// 		add_redirection_node(&filename_token->redirections, new_redir);
// 	remove_token_node(lst, *redir_operator);
// 	// if (get_arr_len(filename_token->contents) == 1)
// 	// 	remove_token_node(lst, filename_token);
// 	*redir_operator = *lst;
// 	return (0);
// }

int	handle_redirection(t_token **lst, t_token **redir_operator)
{
	t_token	*filename_token;
	char	*filename;
	t_token	*redir_cmd;
	t_redir	*new_redir;

	(void)lst;
	if (!(*redir_operator)->next)
		return (-1);
	filename_token = (*redir_operator)->next;
	printf("filename content before gnw = %s\n", filename_token->content);
	if (content_count_words(filename_token->content) > 1)
		filename_token->type = CMD;
	filename = get_next_word(&filename_token->content);
	new_redir = create_redir_node((*redir_operator)->type, filename);
	redir_cmd = find_redir_node(lst, *redir_operator);
	if (!redir_cmd)
	{
		add_redirection_node(&filename_token->redirections, new_redir);
        filename_token->content = malloc(sizeof(char));
        if (!filename_token || gbg_coll(filename_token, PARSING, ADD))
            return (gbg_coll(NULL, ALL, FLUSH_ALL));
        filename_token->content[0] = '\0';
        filename_token->type = CMD;
        
	}
	else
		add_redirection_node(&redir_cmd->redirections, new_redir);
	remove_token_node(lst, *redir_operator);
	print_lst(lst);
	*redir_operator = *lst;
	return (0);
}

t_redir	*get_redir_lst_par(t_token **redir_node_lst)
{
	t_token	*current;
	t_redir	*new_redir_node;
	t_redir	*redir_lst;

	current = *redir_node_lst;
	redir_lst = NULL;
	while (current && (is_a_redir_operator(current)
			|| current->type == OUTFILE))
	{
		if (is_a_redir_operator(current) && current->next
			&& current->next->type == OUTFILE)
		{
			new_redir_node = create_redir_node(REDIR_OUTPUT_APPEND,
					current->next->content);
			remove_token_node(redir_node_lst, current->next);
			remove_token_node(redir_node_lst, current);
			add_redirection_node(&redir_lst, new_redir_node);
			current = *redir_node_lst;
			continue ;
		}
		current = current->next;
	}
	return (redir_lst);
}

void	apply_redir_lst(t_token **lst, t_token *end_node, t_redir **redir_lst)
{
	t_token	*current;
	t_redir	*redir_save;

	current = *lst;
	while (current && current != end_node)
	{
		if (current->type == CMD)
		{
			redir_save = current->redirections;
			// print_redir_lst(&redir_save);
			current->redirections = redir_lst_dup(redir_lst);
			find_last_redir_node(&current->redirections)->next = redir_save;
		}
		current = current->next;
	}
}

int	handle_par_redirection(t_token **lst, t_token **redir_node,
		t_token *closing_par)
{
	t_redir	*redir_lst;
	t_token	*current;
	t_token	*par_right;
	t_token	*next;

	current = *lst;
	par_right = NULL;
	while (current)
	{
		if (current->type == PAR_LEFT)
			par_right = find_closing_par(&current);
		if (par_right == closing_par)
		{
			next = get_outfile_next_node(&closing_par->next);
			redir_lst = get_redir_lst_par(redir_node);
			apply_redir_lst(&current, closing_par, &redir_lst);
			closing_par->next = next;
			return (0);
		}
		current = current->next;
	}
	*redir_node = *lst;
	return (0);
}

int	check_redirections(t_token **lst)
{
	t_token	*current;
	t_token	*prev;

	current = *lst;
	set_redir_lst(lst);
	set_args_lst(lst);
	prev = NULL;
	while (current)
	{
		if (is_a_redir_operator(current) && prev && prev->type == PAR_RIGHT)
		{
			if (handle_par_redirection(lst, &current, prev) == -1)
				return (-1);
			continue ;
		}
		else if (is_a_redir_operator(current))
		{
			if (handle_redirection(lst, &current) == -1)
				return (print_newline_syntax_error());
			continue ;
		}
		prev = current;
		current = current->next;
	}
	return (0);
}
