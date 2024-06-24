/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sabakar- <sabakar-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/24 10:41:49 by sabakar-         ###   ########.fr       */
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
				return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
		}
	}
	return (str);
}

// int	init_data(t_minishell *data, char **envp)
// {
// 	data->path = get_path(envp);
// 	if (!data->path)
// 		return (-1);
// 	data->env_lst = get_env_lst(envp);
// 	if (!data->env_lst)
// 		return (-1);
// 	return (0);
// }

static void	ft_start_execution(t_ast **tree, t_minishell *data)
{
	t_ast	*nodes;
	int		la_status;

	nodes = *tree;
	la_status = ft_start_exec(&nodes, data);
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

int	check_syntax_errors(char *str)
{
	int		i;
	char	c;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\'' || str[i] == '\"')
		{
			c = str[i];
			str = ft_strchr(str + i + 1, c);
			if (!str)
			{
				printf("bash: syntax error near unexpected token `%c'\n", c);
				return (-1);
			}
			i = 0;
			continue ;
		}
	}
	return (0);
}

void	print_redir_lst(t_redir **lst)
{
	t_redir	*current;

	current = *lst;
	while (current)
	{
		printf("redir type = %d\n", current->redir_type);
		printf("redir filename = %s\n", current->filename);
		current = current->next;
	}
}

void	print_lst(t_token **lst)
{
	t_token	*root;

	root = *lst;
	while (root)
	{
		printf("--------\n");
		printf("content = '%s'\n", root->content);
		printf("type = %u\n", root->type);
		if (root->redirections)
			print_redir_lst(&root->redirections);
		root = root->next;
	}
}

void	gbg_delete_node(t_token *node, int mlc_lst)
{
	gbg_coll(node->content, mlc_lst, FREE);
	gbg_coll(node, mlc_lst, FREE);
}


// int     get_nb_of_args(t_token **lst)
// {
//     t_token *current;
//     int args_nb;

//     current = *lst;
//     args_nb = 0;
//     while (current && !is_a_token_operator(current))
//     {
//         if (current->type == ARGS_FLAGS)
//             args_nb++;
//         current = current->next;
//     }
//     return (args_nb);
// }

// int    get_args_flags(t_token **lst)
// {
//     t_token *current;
//     t_token *cmd_node;
//     t_token *next;
//     int nb_of_args;
//     int i;

//     cmd_node = *lst;
//     current = (*lst)->next;
//     nb_of_args = get_nb_of_args(lst);
//     cmd_node->contents = malloc(sizeof(char *) * (nb_of_args + 2));
//     if (!cmd_node->content || gbg_coll(cmd_node->content, PARSING, ADD))
//         return (gbg_coll(NULL, ALL, FLUSH_ALL), -1);
//     cmd_node->contents[0] = msh_strdup(cmd_node->content, PARSING);
//     if (nb_of_args == 0)
//     {
//         cmd_node->contents[1] = NULL;
//         *lst = current;
//         return (0);
//     }
//     i = 0;
//     while (current && current->type == ARGS_FLAGS)
//     {
//         next = current->next;
//         cmd_node->contents[++i] = msh_strdup(current->content, PARSING);
//         remove_token_node(lst, current);
//         current = next;
//     }
//     cmd_node->contents[i + 1] = NULL;
//     *lst = current;
//     return (0);
// }

// void	join_cmd_args(t_token **lst)
// {
// 	t_token	*current;
//     int     i;

//     i = 0;
//     current = *lst;
//     while (current)
//     {
//         if (current->type == CMD)
//         {
//             get_args_flags(&current);
//             continue ;
//         }
//         current = current->next;
//     }
// }

int	start_parsing(char *prompt, t_minishell *data)
{
	t_token	*input;
	t_ast	*tree;
	int		insert_node;

	insert_node = 1;
	if (check_syntax_errors(prompt))
		return (-1);
	input = tokenize_input(prompt);
	if (check_par_syntax(&input) == -1)
		return (-1);
	clean_token_lst(&input);
	split_lst_contents(&input);
	if (check_redirections(&input) == -1)
		return (-1);
	clean_token_lst(&input);
	// join_cmd_args(&input);
	tree = build_ast(&input, &insert_node);
	if (tree)
	{
		// printf("PRINT TREE =========\n");
		// print_tree(&tree);
		// printf("PRINT TREE END =====\n");
		check_tree_syntax(&tree);
	}
    ft_start_execution(&tree, data);
	return (0);
}

void	ft_exit(void)
{
	gbg_coll(NULL, ALL, FLUSH_ALL);
	exit(0);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	data;

	((void)argc, (void)argv);
	data.env_lst = get_env_lst(env);
	while (1)
	{
		data.prompt = readline("./minishell$ ");
		if (!data.prompt | !*data.prompt)
			break ;
		start_parsing(data.prompt, &data);
		free(data.prompt);
	}
	free(data.prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);

}
