/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 17:34:05 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/12 18:14:29 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	main(void)
{
	t_info	info;
	char	*input;

	while (1)
	{
		input = readline("\033[1;32mminishell > \033[0m");
		if (!input)
			break ;
		else
		{
			printf("INPUT: %s\n", input);
			add_history(input);
			evaluate(input, &info);
			free(input);
		}
	}
	printf("Shell ended!\n");
	return (0);
}
