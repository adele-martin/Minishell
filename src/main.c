/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/17 21:32:52 by bschneid         ###   ########.fr       */
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
			data.status = 2;
			data.status_str = ft_itoa(data.status);
			continue ;
		}
		if (build_ast(&data))
			data.status = parse_ast(data.ast_root, &data);
		// if (!restore_stdin_stdout(&data, 2))
		// 	exit (ft_free(&data, 1));
		free_prompt_data(&data);
	}
	ft_printf("exit\n");
	ft_free(&data, 0);
	return (data.status);
}
