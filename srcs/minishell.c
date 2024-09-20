/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kipouliq <kipouliq@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 16:27:00 by kipouliq          #+#    #+#             */
/*   Updated: 2024/09/20 17:05:44 by kipouliq         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

int	ft_start_execution(t_ast **tree)
{
	t_ast	*nodes;

	signal(SIGQUIT, ft_sigquit_handler);
	nodes = *tree;
	if (ft_init_tree(nodes) == -1)
		return (write(2, "\n", 1), ft_shell()->heredoc_sigint = false, -1);
	ft_shell()->pids = NULL;
	ft_shell()->pipes = NULL;
	ft_shell()->end_exec = 0;
	ft_shell()->exec_in_par = 0;
	ft_shell()->expand_chars_trimmed = 0;
	ft_shell()->full_exec_tree = *tree;
	ft_shell()->ft_stdin = dup(STDIN_FILENO);
	ft_shell()->ft_stdout = dup(STDOUT_FILENO);
	if ((ft_shell())->heredoc_sigint)
	{
		ft_close_fds();
		gbg_coll(NULL, ALL, FLUSH_ALL);
		(ft_shell())->heredoc_sigint = false;
		return (-1);
	}
	ft_start_exec(&nodes);
	return (0);
}

int	do_parsing_and_checks(t_token **input)
{
	more_tokenization(input);
	clean_token_lst(input);
	trim_contents(input);
	set_redir_lst(input);
	if (check_redir_syntax(input) == -1 || check_par_syntax(input) == -1)
		return (ft_shell()->exit_status = 2, -1);
	if (check_redirections(input) == -1 || check_pipes_par_syntax(input))
		return (ft_shell()->exit_status = 2, -1);
	return (0);
}

int	start_parsing(char *prompt)
{
	t_token	*input;
	t_ast	*tree;

	if (!prompt[0])
		return (-1);
	if (check_quotes(prompt))
		return (-1);
	input = tokenize_input(prompt);
	if (do_parsing_and_checks(&input) == -1)
		return (-1);
	join_cmd_args(&input);
	clean_token_lst(&input);
	if (check_delete_global_par(&input) == -1)
		return (-1);
	set_par_lst(&input);
	(ft_shell())->les_token = lst_dup(&input, NULL);
	tree = build_ast(&input, NULL);
	if (!tree)
		return (-1);
	if (tree && check_tree_syntax(&tree) == -1)
		return (-1);
	ft_shell()->exec_tree = tree;
	ft_start_execution(&tree);
	ft_close_fds();
	return (0);
}

void	start_all(t_minishell *data)
{
	if (data->prompt[0])
		add_history(data->prompt);
	start_parsing(data->prompt);
	gbg_coll(NULL, PARSING, FLUSH_ONE);
	free(data->prompt);
	close(data->msh_stdout);
}

int	main(int argc, char **argv, char **env)
{
	t_minishell	*data;

	data = ft_shell();
	((void)argc, (void)argv);
	init_data(data, env);
	while (1)
	{
		ft_init_signals();
		data->msh_stdout = open("/dev/stdout", O_RDONLY);
		data->prompt = readline("minishell$ ");
		if (!data->prompt)
		{
			gbg_coll(NULL, ALL, FLUSH_ALL);
			close(data->msh_stdout);
			exit(ft_shell()->exit_status);
		}
		if (data->prompt && *data->prompt)
			start_all(data);
	}
	close(data->msh_stdout);
	free(data->prompt);
	gbg_coll(NULL, ENV, FLUSH_ALL);
	gbg_coll(NULL, PARSING, FLUSH_ALL);
	gbg_coll(NULL, ENV, FREE);
}

// int	main(int argc, char **argv, char **env)
// {
//     t_minishell	*data;
//     data = ft_shell();
//     ((void)argc, (void)argv);
//     init_data(data, env);
//     while (1)
//     {
//         ft_init_signals();
//         data->prompt = readline("minishell$ ");
//         if (data->prompt == NULL)
//         {
//             // Also we need to clean here if I'm not mistaking!
//             // printf("WE ARE HERE IN MAIN\n");
//             gbg_coll(NULL, ALL, FLUSH_ALL);
//             close(ft_shell()->ft_stdin);
//             close(ft_shell()->ft_stdout);
//             ft_putstr_fd("exit\n", 1);
//             exit(ft_shell()->exit_status);
//         }
//         if (data->prompt && *data->prompt)
//         {
//             if (data->prompt[0])
//                 add_history(data->prompt);
//             start_parsing(data->prompt);
//             gbg_coll(NULL, PARSING, FLUSH_ONE);
//             // close(ft_shell()->ft_stdin);
//             // close(ft_shell()->ft_stdout);
//             // gbg_coll(NULL, PARSING, FLUSH_ALL);
//             free(data->prompt);
//         }
//     }
//     free(data->prompt);
//    	close(ft_shell()->ft_stdin);
//     close(ft_shell()->ft_stdout);
//     gbg_coll(NULL, ENV, FLUSH_ALL);
//     gbg_coll(NULL, PARSING, FLUSH_ALL);
//     gbg_coll(NULL, ENV, FREE);
// }

// int	main(int argc, char **argv, char **env)
// {
// 	t_minishell *data;
// 	char *line;

// 	data = ft_shell();
// 	((void)argc, (void)argv);
// 	init_data(data, env);
// 	while (1)
// 	{
// 		ft_init_signals();
// 		if (isatty(fileno(stdin)))
// 			data->prompt = readline("minishell$ ");
// 		else
// 		{
// 			line = get_next_line(fileno(stdin), 0);
// 			data->prompt = ft_strtrim(line, "\n");
// 			free(line);
// 		}
// 		if (!data->prompt)
// 		{
// 			gbg_coll(NULL, ALL, FLUSH_ALL);
// 			exit(ft_shell()->exit_status);
// 		}
// 		if (data->prompt || *data->prompt)
// 		{
// 			if (data->prompt[0])
// 				add_history(data->prompt);
// 			start_parsing(data->prompt);
// 			gbg_coll(NULL, PARSING, FLUSH_ONE);
// 			// close(ft_shell()->ft_stdin);
// 			// close(ft_shell()->ft_stdout);
// 			free(data->prompt);
// 		}
// 	}
// 	free(data->prompt);
// 	gbg_coll(NULL, ENV, FLUSH_ALL);
// 	gbg_coll(NULL, PARSING, FLUSH_ALL);
// 	gbg_coll(NULL, ENV, FREE);
// }