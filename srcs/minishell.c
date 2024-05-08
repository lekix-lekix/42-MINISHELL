/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/05/08 18:23:35 by kipouliq         ###   ########.fr       */
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
	if (!new_node)
		return (NULL);
	new_node->field = malloc(sizeof(char) * (ft_strlen_sep(str, sep) + 1));
	if (!new_node->field)
		return (free(new_node), NULL);
	ft_strlcpy(new_node->field, str, ft_strlen_sep(str, sep) + 1);
	new_node->content = malloc(sizeof(char) * (ft_strlen(sep + 1) + 1));
	if (!new_node->content)
		return (free(new_node->content), free(new_node), NULL);
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
			return (free_env_lst(&lst), NULL); // free lst
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
	if (!env_arr)
		return (NULL);
	i = 0;
	while (node)
	{
		env_arr[i] = ft_strjoin(node->field, node->content);
		if (!env_arr[i])
			return (free_tab_index(env_arr, i), NULL);
		node = node->next;
		i++;
	}
	env_arr[i] = NULL;
	return (env_arr);
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

t_lst	*ft_find_node(t_lst **list, void *mem_addr)
{
	t_lst	*node;

	node = *list;
	while (node)
	{
		if (node->content == mem_addr)
			return (node);
		node = node->next;
	}
	return (NULL);
}

void	add_node_gbg(int which_lst, t_gbg *all_lsts, t_lst *node)
{
	if (which_lst == ENV)
		ft_lstadd_back(&all_lsts->mlc_env, node);
	if (which_lst == PARSING)
		ft_lstadd_back(&all_lsts->mlc_parsing, node);
}

void	remove_free_mem_node(int which_lst, t_gbg *all_lsts, void *mem_addr)
{
	t_lst	*to_remove;

	if (which_lst == ENV)
		to_remove = ft_find_node(&all_lsts->mlc_env, mem_addr);
	if (which_lst == PARSING)
		to_remove = ft_find_node(&all_lsts->mlc_parsing, mem_addr);
	free(to_remove->content);
	free(to_remove);
}

void	flush_lst(t_gbg *all_lsts, int which_list)
{
	if (which_list == ENV)
		ft_lstclear(&all_lsts->mlc_env, free);
	if (which_list == PARSING)
		ft_lstclear(&all_lsts->mlc_parsing, free);
}

int	garbage_collector(void *mem_addr, int which_list, int rule)
{
	static t_gbg	*all_gbg_lsts;
	t_lst			*node;

	node = NULL;
	if (rule == ADD)
	{
		node = ft_lstnew(mem_addr);
		// if (!node) // to do
		// return (garbage_collector(NULL, FLUSH), -1);
		add_node_gbg(which_list, all_gbg_lsts, node);
	}
	if (rule == FREE)
		remove_free_mem_node(which_list, all_gbg_lsts, mem_addr);
	if (rule == FLUSH_ONE)
		flush_lst(all_gbg_lsts, which_list);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_env *lst;
	char **env;
	int i = -1;

	lst = get_env_lst(envp);
	print_real_env(envp);
	print_env(&lst);
	env = env_lst_to_arr(&lst);
	free_env_lst(&lst);
	while (env[++i])
		free(env[i]);
	free(env);
}