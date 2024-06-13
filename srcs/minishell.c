/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/06/13 18:18:14 by kipouliq         ###   ########.fr       */
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

char	*msh_strjoin_space(char const *s1, char const *s2, int mlc_lst)
{
	size_t	s1_size;
	size_t	s2_size;
	char	*final_str;

	final_str = NULL;
	if (!s1)
	{
		final_str = ft_strdup(s2);
		if (!final_str)
			return (NULL);
		return (final_str);
	}
	s1_size = ft_strlen(s1);
	s2_size = ft_strlen(s2);
	final_str = malloc(sizeof(char) * (s1_size + s2_size) + 2);
	if (!final_str || gbg_coll(final_str, mlc_lst, ADD))
		return (gbg_coll(NULL, ALL, FLUSH_ALL), exit(255), NULL);
	ft_strlcpy(final_str, s1, s1_size + 1);
	final_str[s1_size] = ' ';
	ft_strlcpy(final_str + s1_size + 1, s2, s2_size + 1);
	return (final_str);
}

void	join_cmds(t_token **lst)
{
	t_token	*current;
	t_token	*final_node;
    t_token *next;
	char	*final_content;

	current = *lst;
	final_content = NULL;
	while (current)
	{
		while (!is_a_token_operator(current))
        {
			final_content = msh_strjoin_space(final_content, current->content,
					PARSING);
            next = current->next;
            remove_token_node(lst, current);
            current = next;
        }
        final_node = create_cmd_node(final_content, NULL);
        current = current->next;
	}
    
}

int	start_parsing(char *prompt)
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
	printf("TOKEN LST BEFORE REDIR =======\n");
	print_lst(&input);
	printf("============================\n");
	split_lst_contents(&input);
	if (check_redirections(&input) == -1)
		return (-1);
	printf("TOKEN LST BEFORE AST =======\n");
	print_lst(&input);
	printf("============================\n");
	clean_token_lst(&input);
	join_cmds(&input);
	printf("AFTER CLEANING ==== \n");
	// print_lst(&input);
	printf("===========\n");
	tree = build_ast(&input, &insert_node);
	if (tree)
	{
		printf("PRINT TREE =========\n");
		print_tree(&tree);
		printf("PRINT TREE END =====\n");
		check_tree_syntax(&tree);
	}
	return (0);
}

void	ft_exit(void)
{
	gbg_coll(NULL, ALL, FLUSH_ALL);
	exit(0);
}

int	main(int argc, char **argv, char **env)
{
	char	*prompt;

	// char	*path;
	// t_env	*lst;
	(void)argc;
	(void)argv;
	(void)env;
	// path = get_path(env);
	// lst = get_env_lst(env);
	while (1)
	{
		prompt = readline("./minishell$ ");
		if (!prompt || !*prompt || gbg_coll(prompt, PARSING, ADD))
			break ;
		if (ft_strncmp(prompt, "exit", 4) == 0)
			ft_exit();
		start_parsing(prompt);
		gbg_coll(prompt, PARSING, FREE);
		printf("===========\n");
	}
	free(prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}
