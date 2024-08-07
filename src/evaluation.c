/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/06 17:37:23 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	evaluate(char *input, t_info *info)
{
	(void)info;
	char	*path_str;
	char	**cmd_argv;
	char	**bin_paths;
	char	*filepath;
	int		status;
	pid_t	pid;

	printf("Trying to evaluate your given command: %s\n", input);
	if (!*input)
		return (0);
	cmd_argv = ft_split(input, ' ');
	path_str = getenv("PATH");
    if (path_str == NULL)
	{
        perror("getenv");
        exit(EXIT_FAILURE);
	}
	bin_paths = ft_split(path_str, ':');
	while (*bin_paths)
	{
		printf("Searching in: %s\n", *bin_paths);
		filepath = ft_strjoin(*bin_paths, "/");
		filepath = ft_strjoin(filepath, cmd_argv[0]);
		if (access(filepath, X_OK) == 0)
		{
			printf("Found binary at: %s\n", filepath);
			pid = fork();
			if (!pid)
			{
				int val = execve(filepath, cmd_argv, NULL);
				if (val == -1)
					perror("ERROR");
				exit(1);
			}
			else
				break ;
        }
		free(filepath);
		bin_paths++;
	}
	waitpid(-1, &status, 0);
	// lexer(input);
	// parser(input);

	return (status);
}

int	execute(char *input)
{
	char	*path_str;
	char	**cmd_argv;
	char	**bin_paths;
	char	*filepath;
	char	file_exists;
	// int		status;
	// pid_t	pid;

	if (!*input)
		return (0);
	cmd_argv = ft_split(input, ' ');
	if (**cmd_argv == '.')
	{
		if (access(cmd_argv[0], X_OK))
			ft_printf("%s: No execution rights!\n", cmd_argv[0]);
		else
			execve(cmd_argv[0], cmd_argv, NULL);
		exit (2);
	}
	path_str = getenv("PATH");
	if (path_str == NULL)
	{
		perror("getenv");
		exit(EXIT_FAILURE);
	}
	bin_paths = ft_split(path_str, ':');
	file_exists = 0;
	while (*bin_paths)
	{
		// printf("Searching in: %s\n", *bin_paths);
		filepath = ft_strjoin(*bin_paths, "/");
		filepath = ft_strjoin(filepath, cmd_argv[0]);
		if (access(filepath, F_OK) == 0)
			file_exists = 1;
		if (access(filepath, X_OK) == 0)
		{
			execve(filepath, cmd_argv, NULL);
			// printf("Found binary at: %s\n", filepath);
			// pid = fork();
			// if (!pid)
			// {
			// 	int val = execve(filepath, cmd_argv, NULL);
			// 	if (val == -1)
			// 		perror("ERROR");
			// 	exit (1);
			// }
			// else
			// 	break ;
		}
		bin_paths++;
	}
	if (file_exists)
		ft_printf("%s: No execution rights!\n", cmd_argv[0]);
	else
		ft_printf("%s: CMD not found!\n", cmd_argv[0]);
	// waitpid(-1, &status, 0);
	exit (2); //status
}
