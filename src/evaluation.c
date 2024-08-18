/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 22:34:55 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int		run_from_bin_path(char **cmd_argv);
char	**create_argv(t_list *linked_args);

// TODO: else if (is_buildin(cmd_argv[0]))
// 	return (run_buildin(cmd_argv));
int	execute(char *input, t_data *data)
{
	char	**cmd_argv;
	t_list	*linked_args;

	if (!input || !*input)
		return (0);
	linked_args = get_args(input);
	if (!add_wildcards(linked_args))
		return (perror("Error in wildcards"), 1);
	if (!expand_variables(linked_args, data))
		return (perror("Error in expanding variables"), 1);
	cmd_argv = create_argv(linked_args);
	if (!cmd_argv)
		return (1);
	if (**cmd_argv == '.')
	{
		if (access(cmd_argv[0], X_OK))
			ft_printf("%s: No execution rights!\n", cmd_argv[0]);
		else
			execve(cmd_argv[0], cmd_argv, NULL);
		exit (2);
	}
	exit (run_from_bin_path(cmd_argv));
}

char	**create_argv(t_list *linked_args)
{
	int		size;
	t_list	*tmp;
	char	**out;
	char	**writer;

	size = ft_lstsize(linked_args);
	out = (char **)malloc((size + 1) * sizeof(char *));
	if (!out)
		return (NULL);
	writer = out;
	while (linked_args)
	{
		tmp = linked_args;
		clean_quotations(linked_args->content);
		*(writer++) = linked_args->content;
		linked_args = linked_args->next;
		free(tmp);
	}
	*writer = NULL;
	return (out);
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
