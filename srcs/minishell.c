/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/09 16:12:32 by kipouliq         ###   ########.fr       */
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
			if (!str || gbg_coll(str, ENV, ADD) == -1)
				return (NULL);
		}
	}
	return (str);
}

int	free_env_lst(t_env **lst)
{
	t_env	*node;
	t_env	*next;

	node = *lst;
	while (node)
	{
		next = node->next;
		free(node->content);
		free(node->field);
		free(node);
		node = next;
	}
	return (0);
}

int	ft_strlen_sep(char *str, char *sep)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != *sep)
		i++;
	return (i + 1);
}

void	lst_env_add_back(t_env **lst, t_env *new)
{
	t_env	*list;

	list = *lst;
	while (list->next)
		list = list->next;
	if (!new)
		list->next = NULL;
	else
		list->next = new;
}

t_env	*create_env_node(char *str, char *sep)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node || gbg_coll(new_node, ENV, ADD) == -1)
		return (NULL);
	new_node->field = malloc(sizeof(char) * (ft_strlen_sep(str, sep) + 1));
	if (!new_node->field || gbg_coll(new_node->field, ENV, ADD) == -1)
		return (NULL);
	ft_strlcpy(new_node->field, str, ft_strlen_sep(str, sep) + 1);
	new_node->content = malloc(sizeof(char) * (ft_strlen(sep + 1) + 1));
	if (!new_node->content || gbg_coll(new_node->content, ENV, ADD) == -1)
		return (NULL);
	ft_strlcpy(new_node->content, sep + 1, ft_strlen(sep));
	new_node->next = NULL;
	return (new_node);
}

t_env	*get_env_lst(char **envp)
{
	t_env	*lst;
	t_env	*node;
	char	*sep;
	int		i;

	i = -1;
	while (envp && envp[++i])
	{
		sep = ft_strchr(envp[i], '=');
		node = create_env_node(envp[i], sep);
		if (!node)
			return (NULL);
		if (i == 0)
		{
			lst = node;
			continue ;
		}
		else
			lst_env_add_back(&lst, node);
	}
	return (lst);
}

int	init_data(t_minishell *data, char **envp)
{
	data->path = get_path(envp);
	if (!data->path)
		return (-1);
	data->env_lst = get_env_lst(envp);
	if (!data->env_lst)
		return (-1);
	return (0);
}

int	print_env(t_env **lst)
{
	t_env	*node;

	node = *lst;
	while (node)
	{
		printf("field = %s\n", node->field);
		printf("content = %s\n", node->content);
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

int	env_lst_size(t_env **lst)
{
	t_env	*node;
	int		size;

	node = *lst;
	size = 0;
	while (node)
	{
		size++;
		node = node->next;
	}
	return (size);
}

int	free_tab_index(char **tab, int i)
{
	while (i >= 0)
	{
		free(tab[i]);
		i--;
	}
	free(tab);
	return (0);
}

char	**env_lst_to_arr(t_env **lst)
{
	char	**env_arr;
	t_env	*node;
	int		i;

	node = *lst;
	env_arr = malloc(sizeof(char *) * (env_lst_size(lst) + 1));
	if (!env_arr || gbg_coll(env_arr, ENV, ADD) == -1)
		return (NULL);
	i = 0;
	while (node)
	{
		env_arr[i] = ft_strjoin(node->field, node->content);
		if (!env_arr[i] || gbg_coll(env_arr[i], ENV, ADD) == -1)
			return (NULL);
		node = node->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
}

int main(int argc, char **argv, char **env)
{
    (void) argc;
    (void) argv;
    char *path;

    path = get_path(env);
}

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	char *prompt;
// 	t_minishell data;

// 	if (init_data(&data, envp) == -1)
// 		return (-1);
// 	prompt = NULL;
// 	while (1)
// 	{
// 		prompt = readline("./minishell>");
// 		free(prompt);
// 	}
// 	free(data.path);
// }

// int	main(int argc, char **argv, char **envp)
// {
// 	(void)argc;
// 	(void)argv;
// 	(void)envp;
// 	t_env *lst;
// 	char **env;
// 	int i = -1;

// 	lst = get_env_lst(envp);
// 	print_real_env(envp);
// 	print_env(&lst);
// 	env = env_lst_to_arr(&lst);
// 	free_env_lst(&lst);
// 	while (env[++i])
// 		free(env[i]);
// 	free(env);
// 	int *a = malloc(sizeof(int));
// 	if (!a || garbage_collector(a, ENV, ADD))
// 		return (-1);
// 	garbage_collector(a, ENV, FREE);
// 	int *b = malloc(sizeof(int));
// 	if (!b || garbage_collector(b, ENV, ADD))
// 		return (-1);
// 	garbage_collector(b, ENV, FREE);
// 	int i = 0;
// 	while (i < 10)
// 	{
// 		int *a = malloc(sizeof(int));
// 		if (!a || garbage_collector(a, ENV, ADD))
// 			return (-1);
// 		i++;
// 	}
//     int *x = malloc(sizeof(int));
//     char *f = garbage_collector(ft_strdup ("hello"), PARSING, ADD);
//     if (!x || garbage_collector(x, PARSING, ADD) == -1)
//         return (-1);
// 	garbage_collector(NULL, ALL, FLUSH_ALL);
// }