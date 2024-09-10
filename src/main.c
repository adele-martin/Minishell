/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/10 18:25:01 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// saves the status code of the last command
// int	g_signal;
volatile __sig_atomic_t	g_signal;

static char	build_ast(t_data *data);

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	if (initialize_data(&data, argc, argv, envp))
		exit (ft_free(&data, 1));
	while (!data.exit)
	{
		if (!restore_stdin_stdout(&data, 2))
			exit (ft_free(&data, 1));
		handle_signals(1);
		if (isatty(fileno(stdin)))
			data.input = readline("minishell > ");
		else	// for non-interactive mode (testing)
		{
			char *line;
			line = get_next_line(fileno(stdin));
			data.input = ft_strtrim(line, "\n");
			free(line);
		}
		if (!data.input)
			break ;
		else
		{
			handle_signals(2);

			add_history(data.input);			
			if (build_ast(&data))
				g_signal = parse_ast(data.astRoot, &data); // actual execution
			free(data.input);
		}
	}
	// printf("Debug: g_signal = %d\n", g_signal);
	//printf("Shell ended!\n");
	ft_free(&data, 0);
	return (g_signal);
}

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
	data->astRoot = create_ast(data->tokens, end_tokens);
	if (!data->astRoot)
		return (0);
	return (1);
}
