/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/08 15:44:13 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes

#include "../header/minishell.h"


void	builtin_echo(char **argv, int argc)
{
	int	i;
	int option_n;
	int word_count;

	i = 2;
	option_n = 0;
	word_count = argc - 1;
	if (argc >= 2)
	{
	if (argv[1] && ft_strncmp(argv[2], "-n", 2) == 0)
	{
		option_n = 1;
		i++;
	}
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (i == word_count && option_n == 0)
		{
			write(1, "\n", 1);
			break;
		}
		else if (i == word_count && option_n == 1)
			break;
		write(1, " ", 1);
		i++;
	}
	}
}

// void expansion()
// {
// while (argv[i][j])
// 		{
// 			if (argv[i][j] == '$')
// 			{
// 				while (argv[i][j] != ' ')
// 				{
// 					variable[j] = argv[i][j];
// 					j++;
// 				}
// 			}
// }

// void builtin_export(char **argv, char **list_envs)
// {
// 	int	i;

// 	i = 1;
// 	while (argv[i])
// 	{
// 		update_list(argv[i], list_envs);
// 	}
// }

// void wildcard_function()
// {
// }