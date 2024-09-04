/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/04 16:21:33 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// saves the status code of the last command
int	g_signal;

int	main(int argc, char **argv, char **envp)
{
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
		data.input = readline("minishell > ");
		if (!data.input)
			break ;
		else
		{
			add_history(data.input);
			tokens = split_tokens(data.input);
			end_tokens = tokens;
			while (*end_tokens)
				end_tokens++;
			end_tokens--;
			astRoot = create_ast(tokens, end_tokens);
			g_signal = parse_ast(astRoot, &data);
			free(data.status_str);
			data.status_str = ft_itoa(g_signal);
			// waitpid(data.id, &g_signal, 0);
			// printf("Received signal %d\n", g_signal);
			free(data.input);
		}
	}
	printf("Shell ended!\n");
	return (EXIT_SUCCESS);
}
