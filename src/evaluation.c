/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/09 19:56:33 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int	expand_wildcards(char **cmd_argv);
int	run_from_bin_path(char **cmd_argv);

int	execute(char *input)
{
	char	**cmd_argv;

	if (!input || !*input)
		return (0);
	// cmd_argv = ft_split(input, ' ');
	cmd_argv = split_args(input);
	if (!cmd_argv)
		return (1);
	// if (!expand_variables(cmd_argv) || !expand_wildcards(cmd_argv))
	// 	return (1);
	if (**cmd_argv == '.')
	{
		if (access(cmd_argv[0], X_OK))
			ft_printf("%s: No execution rights!\n", cmd_argv[0]);
		else
			execve(cmd_argv[0], cmd_argv, NULL);
		exit (2);
	}
	// else if (is_buildin(cmd_argv[0]))
	// 	return (run_buildin(cmd_argv));
	exit (run_from_bin_path(cmd_argv));
}

int	expand_wildcards(char **cmd_argv)
{
	(void)cmd_argv;
	return (1);
}

int	run_from_bin_path(char **cmd_argv)
{
	char	*path_str;
	char	**bin_paths;
	char	*filepath;
	char	file_exists;

	path_str = getenv("PATH");
	if (!path_str)
	{
		perror("getenv");
		exit(EXIT_FAILURE);
	}
	bin_paths = ft_split(path_str, ':');
	file_exists = 0;
	while (*bin_paths)
	{
		filepath = ft_strjoin(*bin_paths, "/");
		filepath = ft_strjoin(filepath, cmd_argv[0]);
		if (access(filepath, F_OK) == 0)
			file_exists = 1;
		if (access(filepath, X_OK) == 0)
		{
			execve(filepath, cmd_argv, NULL);
		}
		bin_paths++;
	}
	if (file_exists)
		ft_printf("%s: No execution rights!\n", cmd_argv[0]);
	else
		ft_printf("%s: CMD not found!\n", cmd_argv[0]);
	exit (2); //status
}

