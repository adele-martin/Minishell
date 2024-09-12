/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/12 11:26:09 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	g_signal;
// volatile __sig_atomic_t	g_signal;

static void	free_prompt_data(t_data *data);
static char	build_ast(t_data *data);

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	if (initialize_data(&data, argc, envp))
		exit (ft_free(&data, 1));
	while (1) //!data.exit)
	{
		handle_signals(1);
		data.input = readline("minishell > ");
		if (!data.input)
			break ;
		handle_signals(2);
		add_history(data.input);
		if (build_ast(&data))
			g_signal = parse_ast(data.astRoot, &data); // actual execution
		free_prompt_data(&data);
		if (!restore_stdin_stdout(&data, 2))		// needs to be set later again, but differently!!!
			exit (ft_free(&data, 1));
	}
	ft_free(&data, 0);
	return (g_signal);
}
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
	// if (data->astRoot)
	// {
	// 	free_ast(data->astRoot);	// TODO: FREE FUNC FOR AST
	// 	data->astRoot = NULL;
	// }

	// if (data->linked_args)			// free linked_args
		// ft_lstclear(&data->linked_args, free);
	
	if (data->cmd_argv)				// free cmd_argv STILL SEGFAULTS
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
	// print_tokens(data->tokens);
	end_tokens = data->tokens;
	while (*end_tokens)
		end_tokens++;
	end_tokens--;
	data->astRoot = create_ast(data->tokens, end_tokens);
	if (!data->astRoot)
		return (0);
	return (1);
}
