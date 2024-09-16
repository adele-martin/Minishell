/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/16 16:31:51 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

volatile __sig_atomic_t	g_signal;

int	main(int argc, char **argv, char **envp)
{
	t_data	data;

	(void)argv;
	if (!initialize_data(&data, argc, envp))
		exit (1);
	while (1)
	{
		data.input = readline("minishell > ");
		if (!data.input)
			break ;
		handle_signals(2);
		add_history(data.input);
		if (!check_input(data.input, 0))
		{
			free(data.input);
			g_signal = 2;
			continue ;
		}
		if (build_ast(&data))
			g_signal = parse_ast(data.ast_root, &data);
		free_prompt_data(&data);
	}
	ft_printf("exit\n");
	ft_free(&data, 0);
	return (g_signal);
}
