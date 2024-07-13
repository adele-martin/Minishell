/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/07/12 18:14:18 by bschneid         ###   ########.fr       */
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
	pid_t	pid;

	printf("Trying to evaluate your given command: %s\n", input);
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
	// lexer(input);
	// parser(input);

	return (0);
}
