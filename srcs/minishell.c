/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/07 18:28:16 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

char	*get_path(char **envp)
{
	char	*str;
	int		i;

	i = -1;
	while (envp && envp[++i])
	{
		if (!ft_strncmp(envp[i], "PATH=", 5))
		{
			str = ft_strtrim(envp[i], "PATH=");
			if (!str)
				return (NULL);
		}
	}
	return (str);
}

int     ft_strlen_sep(char *str, char c)
{
    int i;

    i = 0;
    while (str[i] && str[i] != c)
        i++;
    return (i);
}

// t_env	*get_env_lst(char **envp)
// {
// 	t_env	*lst;
// 	t_env	*new_node;
//     char    *sep;
// 	int		i;

//     new_node = malloc(sizeof(t_env));
//     if (!new_node)
//         return (NULL);
//     i = -1;
//     while (envp && envp[++i])
//     {
//         sep = ft_strchr(envp[i], '=');
//         new_node->content = malloc(sizeof(char) * (ft_strlen_sep(envp[i], '=')) + 1);
//         if (!new_node->content)
//             return (NULL);
//         new_node->field = malloc(sizeof(char) * (ft_strlen_sep(envp[i], '\0')) + 1);
//         if (!new_node->field)
//             return (NULL);
//     }
// }

int	init_data(t_minishell *data, char **envp)
{
	data->path = get_path(envp);
	if (!data->path)
		return (-1);
	// data->env_lst = get_env_lst(envp);
	return (0);
}

int	print_env(char **envp)
{
	int	i;

	i = -1;
	while (envp && envp[++i])
		printf("%s\n", envp[i]);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	char *prompt;
	t_minishell data;

	if (init_data(&data, envp) == -1)
		return (-1);
	prompt = NULL;
	while (1)
	{
		prompt = readline("minishell>");
		free(prompt);
	}
	free(data.path);
}