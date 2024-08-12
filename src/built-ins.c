/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/12 11:20:30 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes

#include "../header/minishell.h"

void	builtin_echo(char **argv, int argc)
{
	int	i;
	int option_n;

	option_n = 0;
	if (argc > 1 && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		option_n = 1;
		i = 2;
	}
	else
		i = 1;
	while (argv[i])
	{
		ft_putstr_fd(argv[i], 1);
		if (argv[i + 1])
			write(1, " ", 1);
		i++;
	}
	if (!option_n)
		write(1, "\n", 1);
}

//TODO: finish unset variable
void builtin_unset(char **argv, char **list_envs)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		delete_var(argv[i], list_envs);
		i++;
	}
	// i = 0;
	// while (list_envs[i])
	// {
	// 	ft_printf("%s\n", list_envs[i]);
	// 	i++;
	// }
}

void builtin_env(char **argv, int argc, char **list_envs)
{
	int	i;
	(void)argv;
	(void)argc;

	i = 0;
	while (list_envs[i])
	{
		ft_printf("%s\n", list_envs[i]);
		i++;
	}
}

//TODO: perhaps add a second array of intermediary variables that have not yet entered the env array;
void builtin_export(char **argv, int argc, char **list_envs)
{
	int i;

	i = 1;
	if (argc >= 1)
	{
		while (argv[i])
		{
			update_list(argv[i], list_envs);
			i++;
		}
	}
	else
	{
	i = 0;
	while (list_envs[i])
	{
		ft_printf("%s\n", list_envs[i]);
		i++;
	}
	}

}

// void wildcard_function()
// {
// }