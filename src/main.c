/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/10 15:39:26 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// saves the status code of the last command
// int	g_signal;
volatile __sig_atomic_t	g_signal;

int	main(int argc, char **argv, char **envp)
{
	//char **list_envs = envs_list(envp);
	//(void)envp;
	//builtin_echo(argv, argc);

	char	**tokens;
	char	**end_tokens;
	t_ast	*astRoot;
	t_data	data;

	if (initialize_data(&data, argc, argv, envp))
		return (EXIT_FAILURE);
	while (!data.exit)
	{
		if (!restore_stdin_stdout(&data, 2))
			exit (1);
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
			tokens = split_tokens(data.input);
			end_tokens = tokens;
			while (*end_tokens)
				end_tokens++;
			end_tokens--;
			astRoot = create_ast(tokens, end_tokens);
			if (!astRoot)
				continue ;
			g_signal = parse_ast(astRoot, &data); // actual execution
			free(data.input);
		}
	}
	// printf("Shell ended!\n");
	return (2);
}
