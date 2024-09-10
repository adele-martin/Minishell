/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built_ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/07 12:32:57 by ademarti          #+#    #+#             */
/*   Updated: 2024/09/10 11:24:44 by ademarti         ###   ########.fr       */
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

// prints out the arguments passed to it
// includes the option -n, which suppresses the trailing newline
int	builtin_echo(char **argv, int argc)
{
	int	i;
	int option_n;

	option_n = 0;
	if (argc > 1 && ft_strncmp(argv[1], "-n", 3) == 0)
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

// prints out the current environments (saved in list_envs)
int builtin_env(char **list_envs)
{
	while (*list_envs)
		ft_printf("%s\n", *(list_envs++));
	return (0);
}

int wildcard_match(const char *pattern, const char *str)
{
    if (*pattern == '\0' && *str == '\0')  // Both strings empty -> match
        return true;
    if (*pattern == '*' && *(pattern + 1) == '\0')  // Pattern ends with * -> match
        return true;
    if (*pattern == '*' && wildcard_match(pattern + 1, str))  // * matches 0 characters
        return true;
    if (*pattern == '*' && *str && wildcard_match(pattern, str + 1))  // * matches 1 character
        return true;
    if (*pattern == '?' && *str)  // ? matches exactly 1 character
        return wildcard_match(pattern + 1, str + 1);
    if (*pattern == *str)  // Exact match for current character
        return wildcard_match(pattern + 1, str + 1);

    return 1;
}

int builtin_export(char **argv, int argc, char **list_envs, t_list *export_list)
{
	int i;
	i = 1;
	printf("Processing argument: %s\n", argv[i]);
	if (argc >= 2)
    {
        while (argv[i])
        {
            // Check for wildcards inside the loop for each argument
			/*
            int is_wildcard = (ft_strchr(argv[i], '*') || ft_strchr(argv[i], '?'));

            if (is_wildcard)
            {
                for (int j = 0; list_envs[j]; j++)
                {
                    if (wildcard_match(argv[i], list_envs[j]))
                    {
                        // Process matched environment variable
                        if (has_equalsign(list_envs[j]) == 1)
                        {
                            update_list(list_envs[j], list_envs);
                            fill_exportlist(list_envs[j], &export_list);
                        }
                        else if (has_equalsign(list_envs[j]) == 0)
                        {
                            fill_exportlist(list_envs[j], &export_list);
                        }
                    }
                }
            }
			*/
                if (has_equalsign(argv[i]) == 1)
                {
                    update_list(argv[i], list_envs);
                    fill_exportlist(argv[i], &export_list);
                }
                else if (has_equalsign(argv[i]) == 0)
                {
                    fill_exportlist(argv[i], &export_list);
                }
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

// prints out the current working directory
int builtin_pwd(void)
{
	char cwd[1024];

	getcwd(cwd, sizeof(cwd));
	ft_printf("%s\n", cwd);
	return (0);
}

// with no arguments, exits with status 0
// with args, checks for numeric (first) argument, optional error message
// if more than one argument, prints error
// if one argument, exits with that status
int builtin_exit(char **argv, int argc)
{
	int	i;

	i = 0;
	if (argc == 1)
		exit(0);
	while (argv[1])
	{
		if (!ft_isalnum(argv[1][i]))
		{
			ft_printf("minishell: exit: %s: numeric argument required\n", argv[1]);
			exit(2);
		}
		i++;
	}
	ft_printf("exit\n");
	if (argc > 2)
	{
		ft_printf("minishell: exit: too many arguments\n");
		return (1);
	}
	exit(ft_atoi(argv[1]));
}

//Helper for cd built-in to update PWD and OLDPWD
int update_env_var(const char *key, const char *value, char **list_envs)
{
    char env_var[1024];

	ft_strlcpy(env_var, key, sizeof(env_var));
	ft_strcat(env_var, value);
	update_list(env_var, list_envs);
	return 0;
}

//Helper for cd built-in for a change to home directory
int change_to_home(char **list_envs) {
	 char	*home_dir;

	home_dir = search_env("HOME", list_envs);
	if (!home_dir) {
		ft_printf("minishell: cd: HOME not set\n");
		return -1;
	}
	if (chdir(home_dir) == -1) {
		ft_printf("minishell: cd: error changing to HOME directory\n");
		return -1;
	}
	return 0;
}

// TODO: also change the OLDPWD - variable - this function is also way tooo messy!!!
int builtin_cd (char **argv, int argc, char **list_envs)
{
	char cwd[1024];
	char *current_dir;
	char *previous_pwd;

	previous_pwd = search_env("PWD", list_envs);
	if (argc == 1 && (change_to_home(list_envs) == -1))
			return (1);
	else if (argc == 2 && chdir(argv[1]) == -1)
			ft_printf("minishell: cd: %s: No such file or directory\n", argv[1]);
	else if (argc > 2)
	{
		ft_printf("minishell: cd: too many arguments\n");
		return (1);
	}
	current_dir = getcwd(cwd, sizeof(cwd));
	if (update_env_var("OLDPWD=", previous_pwd, list_envs) == -1 || update_env_var("PWD=", current_dir, list_envs) == -1)
		return (1);
	return (0);
}
