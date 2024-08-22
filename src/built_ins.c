/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/22 18:01:46 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes

#include "../header/minishell.h"

/*TODO: return values of the built-ins
0: Success
1: General error
2: Misuse of shell built-ins (e.g., syntax errors)
126: Command invoked cannot execute
127: Command not found
130: Script terminated by Ctrl+C (SIGINT)
We can store in into a global variable or into
int built_ins()
{
	if ()
	else
		return (127);
}
*/

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
		if (search_var(argv[i], list_envs, head) != NULL)
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

//TODO: Export from linked list to array. After exported delete it from linked list --> this doesn't exactly work because 'export hey' is still printed with export command!
int builtin_export(char **argv, int argc, char **list_envs)
{
	int i;
	int j;

	i = 1;
	j = 0;
	if (argc >= 1)
	{
		while (argv[i])
		{

			if (has_equalsign(argv[i]) == 1)
				update_second_list();
				update_list(argv[i], list_envs);//it is updated to the env list
			else
				//it is updated to the export list only
			i++;
		}
	}
	else
	{
	//sort list
	//print list
	}
	return (0);
}

int builtin_pwd(char **argv, char **list_envs)
{
	(void)argv;
	update_list(getenv("PWD"), list_envs);
	ft_printf("\n");
	return (0);
}

int builtin_exit(char **argv, int argc)
{
	(void)argv;
	(void)argc;
	ft_printf("exit\n");
	exit(1);
	return (0);
}

char *ft_strcat(char* dest, const char* src) {
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
	char old_pwd[1024];

	if (argc == 1)
	{
		home = getenv("HOME");
		if (!home)
			ft_printf("bash: cd: HOME not set\n");
		if (chdir(home) == -1)
			ft_printf("error");
	}
	else if (argc == 2)
	{
	if (chdir(argv[1]) == -1)
		ft_printf("cd: %s: No such file or directory\n", argv[1]);
	}
	else if (argc > 2)
	{
		ft_printf("bash: cd: too many arguments\n");
	}
	current_dir = getcwd(cwd, sizeof(cwd));
	ft_strlcpy(old_pwd, "OLDPWD=", sizeof(old_pwd));
	ft_strcat(old_pwd, current_dir);
	update_list(old_pwd, list_envs);
	update_list(getenv("PWD"), list_envs);
	return (0);
}
