/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/07 15:29:34 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes

#include "../header/minishell.h"

// void	builtin_echo(char **argv)
// {
// 	int	i;
// 	int j;
// 	char *variable;
// 	int option_n;

// 	i = 1;
// 	option_n = 0;
// 	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
// 		option_n = 1;
// 	while (argv[i])
// 	{
// 		j = 0;
// 		while (argv[i][j])
// 		{
// 			if (argv[i][j] == '$')
// 			{
// 				while (argv[i][j] != ' ')
// 				{
// 					variable[j] = argv[i][j];
// 					j++;
// 				}
// 			}
// 		write(1, &argv[i][j], 1);
// 		j++;
// 		}
// 		i++;
// 	}
// }

void builtin_export(char **argv, char **list_envs)
{
	int	i;

	i = 1;
	update_list(argv[i], list_envs);
}