/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_split_token_content.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 17:46:45 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/03 13:24:44 by sabakar-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	get_end_quote(char *str, int i, char c)
{
	while (str && str[i] != c)
		i++;
	return (i);
}

int	content_count_words(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		while (str[i] && ft_is_space(str[i]))
			i++;
		if (str[i])
			count++;
		if (is_quote(str[i]))
			i = get_end_quote(str, i + 1, str[i]) + 1;
		while (str[i] && !ft_is_space(str[i]))
		{
			if (is_quote(str[i]))
				i = get_end_quote(str, i + 1, str[i]);
			i++;
		}
	}
	return (count);
}

char	*get_quotes_block(char *str, char **input_str, int i)
{
	char	*word;
	char	*sep;

	sep = ft_strchr(str + i + 1, str[i]) + 1;
	word = malloc(sizeof(char) * (ft_strlen_sep(str + i, sep) + 1));
	if (!word || gbg_coll(word, PARSING, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strcpy_sep_ptr(word, str + i, sep);
	*input_str = sep + 1;
	return (word);
}

int	get_end_word_idx(char *str, int i)
{
	while (str[i])
	{
		// if (i > 0 && is_quote(str[i]))
		// 	return (i);
		if (is_quote(str[i]))
			i = get_end_quote(str, i + 1, str[i]);
		if (ft_is_space(str[i]))
			return (i);
		i++;
	}
	return (i);
}

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
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strcpy_sep(word, str + i, str + j);
	*input_str = str + j + 1;
	return (word);
}

int	split_lst_contents(t_token **lst)
{
	t_token	*current;
	char	*content_cpy;
	int		words_count;
	int		i;

	current = *lst;
	if (!current)
		return (-1);
	while (current)
	{
		if (current->content)
		{
			words_count = content_count_words(current->content);
			// dprintf(2, "words count = %d\n", words_count);
			if (!words_count)
			{
				content_cpy = malloc(sizeof(char));
				if (!content_cpy || gbg_coll(content_cpy, PARSING, ADD))
					return (gbg_coll(NULL, ALL, FLUSH_ALL));
				content_cpy[0] = '\0';
			}
			else
				content_cpy = msh_strdup(current->content, PARSING);
			current->contents = malloc(sizeof(char *) * (words_count + 1));
			if (!current->contents || gbg_coll(current->contents, PARSING, ADD))
				return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), -1);
			i = -1;
			while (++i < words_count)
			{
				current->contents[i] = get_next_word(&content_cpy);
				// dprintf(2, "current->contents[%d] = %s\n", i,
					// current->contents[i]);
			}
			current->contents[words_count] = NULL;
		}
		current = current->next;
	}
	return (0);
}
