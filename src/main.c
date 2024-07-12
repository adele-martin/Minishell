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
		input = readline("minishell > ");
		if (!input)
			break ;
		else
			add_history(input);
		printf("INPUT: %s\n", input);
		evaluate(input, &info);
	}
	printf("Shell ended!\n");
	return (0);
}
