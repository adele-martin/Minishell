/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/21 16:46:05 by ademarti         ###   ########.fr       */
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

}

void builtin_pwd(char **argv, char **list_envs)
{
	(void)argv;
	update_list(getenv("PWD"), list_envs);
	// ft_printf(getenv("PWD"));
	ft_printf("\n");
}

void builtin_exit(char **argv, int argc)
{
	(void)argv;
	(void)argc;
	ft_printf("exit\n");
	exit(1);
}

// void change_path()
// {

// }

//TO DO: Check if the "cd " command works + do pwd function first
// void builtin_cd (char **argv, int argc, char **list_envs)
// {
// 	// char cwd[1024];
// 	// char *current_dir = getcwd(cwd, sizeof(cwd));
// 	// char old_pwd[1024]; //put them in other function
// 	// ft_strlcat(old_pwd, current_dir, sizeof(old_pwd));
// 	// update_list(old_pwd, list_envs);
// 	if (argc == 2)
// 	{
// 	if (ft_strncmp(argv[1], "..", 2) == 0)
// 	{
// 		if (chdir("..") != 0)
// 			perror("chdir() error");
// 	}
// 	else if (chdir(argv[1]) == -1)
// 		ft_printf("cd: %s: No such file or directory\n", argv[1]);
// 	}
// 	else if (argc == 1)
// 	{
// 	if (chdir(getenv("HOME")) != 0)
// 		printf("error");
// 	}
// 	else
// 		ft_printf("bash: cd: too many arguments\n");
// 	// builtin_pwd(argv, list_envs);
// 	int i = 0;
// 	while (list_envs[i])
// 	{
// 		printf("%s\n", list_envs[i]);
// 		i++;
// 	}
// }

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

void builtin_cd (char **argv, int argc, char **list_envs)
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
}
