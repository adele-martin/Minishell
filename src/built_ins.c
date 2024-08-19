/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/19 12:10:53 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes

#include "../header/minishell.h"


void builtin_cd(char **argv, int argc)
{
	(void)argv;
	(void)argc;
	ft_printf("<TEST in BUILDIN CD>\n");
}

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
	ft_printf("<TEST in BUILDIN ECHO>\n");
}


//TO DO: delete variable from linked list
void builtin_unset(char **argv, char **list_envs, t_list *head)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		if (delete_env(argv[i], list_envs))
		i++;
	}
	if (search(argv[i], list_envs, head) != NULL)
	(void)head;
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
	ft_printf("<TEST in BUILDIN ENV>\n");
}

//TODO: xport from linked list to array. After exported delete it from linked list
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
	ft_printf("<TEST in BUILDIN ECHO>\n");
}

void builtin_pwd(char **argv, int argc)
{
	(void)argv;
	(void)argc;
	ft_printf(getenv("PWD"));
	ft_printf("\n");
	ft_printf("<TEST in BUILDIN PWD>\n");
}

void builtin_exit(char **argv, int argc)
{
	(void)argv;
	(void)argc;
	ft_printf("exit\n");
	exit(1);
	ft_printf("<TEST in BUILDIN EXIT>\n");
}


//TO DO: Check if the "cd " command works + do pwd function first
// void builtin_cd(char **argv, int argc)
// {
// char *curr_dir = getcwd();
// 	if (argc == 2)
// 	{
// 	if (ft_strncmp(argv[1], "..", 2) == 0)
// 		ft_printf("yeah");
// 	else if (chdir(argv[1]) != 0)
// 		ft_printf("cd: %s: No such file or directory\n", argv[1]);
// 	}
// 	else if (argc == 1)
// 	{
// 	if (chdir(getenv("HOME")) != 0)
// 		printf("error");
// 	}
// 	else
// 	{
// 		ft_printf("bash: cd: too many arguments\n");
// 	}
// }

