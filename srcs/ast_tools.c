/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_tools.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 13:53:14 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/22 14:43:02 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

// void	parse_clear_tree(t_token *root)
// {
// 	if (!root)
// 		return ;
// 	if (root->left)
// 		parse_clear_tree(root->left);
// 	if (root->right)
// 		parse_clear_tree(root->right);
// 	if (root->content)
// 		gbg_coll(root->content, PARSING, FREE);
// 	if (root)
// 		gbg_coll(root, PARSING, FREE);
// }

// void	clear_tree(t_token **root)
// {
// 	t_token	*node;

// 	if (!root || !*root)
// 		return ;
// 	node = *root;
// 	parse_clear_tree(node);
// }

int	ft_strlen_sep(char *str, char *sep)
{
	int	i;

	i = 0;
	if (!sep)
		return (ft_strlen(str));
	while (str[i] && str[i] != *sep)
		i++;
	return (i + 1);
}

int	is_an_operator(char c)
{
	return (c == '|' || c == '&' || c == '(' || c == ')');
}

char	*find_operator(char *str)
{
	int	i;

	i = -1;
	if (!str | !str[0])
		return (NULL);
	while (str[++i])
	{
		if (is_an_operator(str[i]))
			return (str + i);
	}
	return (NULL);
}

int	ft_strcpy_sep(char *dest, char *input, char *sep)
{
	int	i;

	i = -1;
	if (!sep)
		return (ft_strlcpy(dest, input, ft_strlen(input) + 1));
	while (input[++i] && input[i] != sep[0])
		dest[i] = input[i];
	dest[i] = '\0';
	return (i);
}
