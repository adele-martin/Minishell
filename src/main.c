/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/11 14:34:37 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// saves the status code of the last command
int	g_signal;
// volatile __sig_atomic_t	g_signal;

static char	build_ast(t_data *data);

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (initialize_data(&data, argc, argv, envp))
		exit (ft_free(&data, 1));
	while (!data.exit)
	{
		handle_signals(1);
		data.input = readline("minishell > ");
		if (!data.input)
			break ;
		handle_signals(2);
		add_history(data.input);
		if (build_ast(&data))
			g_signal = parse_ast(data.astRoot, &data); // actual execution
		ft_split_free(data.tokens);
		free(data.input);
		if (!restore_stdin_stdout(&data, 2))		// needs to be set later again, but differently!!!
			exit (ft_free(&data, 1));
	}
	ft_free(&data, 0);
	return (g_signal);
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
