/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/10 16:17:13 by ademarti         ###   ########.fr       */
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
		exit (ft_free(&data, 1));
	while (!data.exit)
	{
		if (!restore_stdin_stdout(&data, 2))
			exit (ft_free(&data, 1));
		handle_signals(1);
		data.input = readline("minishell > ");
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
			//free(data.input);
		}
	}
	ft_free(&data, 0);
	//printf("Shell ended!\n");
	return (g_signal);
}
