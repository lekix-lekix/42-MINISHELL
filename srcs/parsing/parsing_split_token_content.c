/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_split_token_content.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:46:45 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/11 20:39:07 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_next_word(char **input_str)
{
	char	*word;
	char	*str;
	int		i;
	int		j;

	i = 0;
	str = *input_str;
	if (!str || !str[0])
		return (NULL);
	while (str[i] && ft_is_space(str[i]))
		i++;
	j = get_end_word_idx(str, i);
	word = malloc(sizeof(char) * (ft_strlen_sep(str + i, str + j) + 1));
	if (!word || gbg_coll(word, PARSING, ADD))
		return (ft_exit_close(255), NULL);
	ft_strcpy_sep_ptr(word, str + i, str + j);
	*input_str = str + j + 1;
	return (word);
}

int	allocate_fill_contents(t_token *current, char *content_cpy, int words_count)
{
	int	i;

	current->contents = malloc(sizeof(char *) * (words_count + 1));
	if (!current->contents || gbg_coll(current->contents, PARSING, ADD))
		return (ft_exit_close(255), -1);
	i = -1;
	while (++i < words_count)
		current->contents[i] = get_next_word(&content_cpy);
	current->contents[words_count] = NULL;
	return (0);
}

int	split_lst_contents(t_token **lst)
{
	t_token	*current;
	char	*content_cpy;
	int		words_count;

	current = *lst;
	if (!current)
		return (-1);
	while (current)
	{
		if (current->content)
		{
			words_count = content_count_words(current->content);
			if (!words_count)
				content_cpy = empty_str();
			else
				content_cpy = msh_strdup(current->content, PARSING);
			allocate_fill_contents(current, content_cpy, words_count);
		}
		current = current->next;
	}
	return (0);
}
