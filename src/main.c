/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 19:30:54 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// int	g_signal;
volatile __sig_atomic_t	g_signal;

static void	free_prompt_data(t_data *data);
static char	build_ast(t_data *data);

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	if (initialize_data(&data, argc, envp))
		exit (ft_free(&data, 1));
	while (1)
	{
		handle_signals(1);
		data.input = readline("minishell > ");
		if (!data.input)
			break ;
		handle_signals(2);
		add_history(data.input);
		if (build_ast(&data))
			g_signal = parse_ast(data.ast_root, &data);
		free_prompt_data(&data);
		if (!restore_stdin_stdout(&data, 2))
			exit (ft_free(&data, 1));
	}
	ft_free(&data, 0);
	return (g_signal);
}

// TODO: Implement function to free the AST and linked_args
// build specific functions for freeing the data in each while loop
static void	free_prompt_data(t_data *data)
{
	if (data->input)
	{
		free(data->input);
		data->input = NULL;
	}
	if (data->tokens)
	{
		ft_split_free(data->tokens);
		data->tokens = NULL;
	}
	if (data->cmd_argv)
		free_array(&data->cmd_argv);
}

// static void	print_tokens(char **tokens)
// {
// 	ft_printf("Tokens:\n");
// 	while (*tokens)
// 	{
// 		ft_printf("%s\n", *tokens);
// 		tokens++;
// 	}
// }

// splits the input into tokens and builds the AST
// returns 1 on success, 0 on empty AST
static char	build_ast(t_data *data)
{
	char	**end_tokens;

	data->tokens = split_tokens(data->input);
	if (!data->tokens)
		return (0);
	end_tokens = data->tokens;
	while (*end_tokens)
		end_tokens++;
	end_tokens--;
	data->ast_root = create_ast(data->tokens, end_tokens);
	if (!data->ast_root)
		return (0);
	return (1);
}
