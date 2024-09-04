/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/09/04 12:10:28 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*TODO: return values of the built-ins
0: Success
1: General error
2: Misuse of shell built-ins (e.g., syntax errors)
126: Command invoked cannot execute
127: Command not found
130: Script terminated by Ctrl+C (SIGINT)
We can store in into a global variable or into

*/
// int built_ins()
// {
// 	if ()
// 		return (builtin_echo(argv, argc))
// 	else if ()
// 	else
// 		return (127);
// }

int	builtin_echo(char **argv, int argc)
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
	return (0);
}

int builtin_unset(char **argv, char **list_envs, t_list *head)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		delete_env(argv[i], list_envs);
		i++;
	}
	i = 0;
	while (argv[i])
	{
		if (search_var(argv[i], head) != NULL)
			delete_var(argv[i], head);
		i++;
	}
	return (0);
}

int builtin_env(char **argv, int argc, char **list_envs)
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
	return (0);
}

int has_equalsign(char *string)
{
	int i = 0;
	while (string[i])
	{
		if (string[i] == '=')
			return (1);
		i++;
	}
	return (0);
}


int builtin_export(char **argv, int argc, char **list_envs, t_list *export_list)
{
	int i;

	i = 1;
	if (argc >= 2)
	{
		while (argv[i])
		{
			if (has_equalsign(argv[i]) == 1)
			{
				update_list(argv[i], list_envs);
				fill_exportlist(argv[i], &export_list);
			}
			else if (has_equalsign(argv[i]) == 0)
				fill_exportlist(argv[i], &export_list);
			i++;
		}
	}
	else
	{
	sortList(export_list);
	printList(export_list);
	}
	return (0);
}

int builtin_pwd(char **argv, char **list_envs)
{
	(void)argv;
	ft_printf("%s\n", search_env("PWD", list_envs));
	return (0);
}

int builtin_exit(char **argv, int argc)
{
	(void)argv;
	(void)argc;
	int i = 0;
	while (argv[1])
	{
		if (!ft_isalnum(argv[1][i]))
		{
			ft_printf("bash: exit: numeric argument required\n");
			exit(1);
		}
		i++;
	}
	ft_printf("exit\n");
	exit(1);
	return (0);
}

char *ft_strcat(char* dest, const char* src)
{
	char* ptr = dest;
	while (*ptr != '\0') {
		ptr++;
	}
	while (*src != '\0') {
		*ptr = *src;
		ptr++;
		src++;
	}
	*ptr = '\0';
	return dest;
}

int builtin_cd (char **argv, int argc, char **list_envs)
{
	char *home;
	char cwd[1024];
	char *current_dir;
	char new_pwd[1024];
	char old_pwd[1024];

	char *previous_pwd;
	previous_pwd = search_env("PWD", list_envs);
	if (argc > 2)
	{
		ft_printf("minishell: cd: too many arguments\n");
		return (1);
	}
	else if (argc == 1)
	{
		home = search_env("HOME", list_envs);
		if (!home)
			return (ft_printf("minishell: cd: HOME not set\n"));
		if (chdir(home) == -1)
			ft_printf("error");
	}
	else if (argc == 2)
	{
		if (chdir(argv[1]) == -1)
			ft_printf("minishell: cd: %s: No such file or directory\n", argv[1]);
	}
	current_dir = getcwd(cwd, sizeof(cwd)); // TODO: hasn't this to be taken from the list_envs? No I use this variable to update pwd in the list_envs

	ft_strlcpy(new_pwd, "PWD=", sizeof(new_pwd));
	ft_strcat(new_pwd, current_dir);
	update_list(new_pwd, list_envs);
	ft_strlcpy(old_pwd, "OLDPWD=", sizeof(old_pwd));
	ft_strcat(old_pwd, previous_pwd);
	update_list(old_pwd, list_envs);
	return (0);
}
