/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 21:53:40 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static char	init_execution(char *input, t_data *data);
static int	run_extern(t_data *data);
static int	action_in_child(t_data *data);

// 	return (run_buildin(cmd_argv));
int	execute(char *input, t_data *data)
{
	if (!init_execution(input, data))
		return (0);
	if (!add_wildcards(data, data->linked_args))
		return (error_message(NULL, NULL, "Error in wildcards"), 1);
	if (!expand_variables(data->linked_args, data))
		return (error_message(NULL, NULL, "Error in expanding variables"), 1);
	if (!clean_args(&data->linked_args))
		return (0);
	if (!create_argv_argc(data, data->linked_args))
		return (1);
	if (!run_builtin(data))
		data->status = run_extern(data);
	else if (!data->in_child)
		return (data->status);
	waitpid(data->id, &data->status, 0);
	if (WIFEXITED(data->status))
		data->status = WEXITSTATUS(data->status);
	return (data->status);
}

// Initializes the linked_args list for execution
static char	init_execution(char *input, t_data *data)
{
	if (data->signal_fd)
	{
		write(data->signal_fd, &data->stdin, sizeof(int));
		close(data->signal_fd);
	}
	if (!input || !*input)
		return (0);
	data->linked_args = get_args(input);
	if (!data->linked_args)
		return (0);
	return (1);
}

static int	run_extern(t_data *data)
{
	if (!data->in_child)
		data->id = fork();
	if (data->id == -1)
		return (1);
	if (data->in_child || data->id == 0)
	{
		handle_signals(3);
		exit (ft_free(data, action_in_child(data)));
	}
	waitpid(data->id, &data->status, 0);
	if (WIFEXITED(data->status))
		data->status = WEXITSTATUS(data->status);
	return (data->status);
}

static int	action_in_child(t_data *data)
{
	struct stat	path_stat;

	if (ft_strncmp(data->argv[0], "~/", 2) == 0)
		update_home(data, data->argv);
	if (**data->argv == '.' || **data->argv == '/')
	{
		if (stat(data->argv[0], &path_stat) != 0)
		{
			error_message(data->argv[0], NULL, "No such file or directory");
			return (127);
		}
		if (S_ISDIR(path_stat.st_mode))
			return (error_message(data->argv[0], NULL, "Is a directory"), 126);
		if (access(data->argv[0], X_OK) == 0)
			exit(ft_free(data,
					execve(data->argv[0], data->argv, data->list_envs)));
		return (error_message(data->argv[0], NULL, "Permission denied"), 126);
	}
	return (ft_free(data, run_from_bin_path(data)));
}

void	update_home(t_data *data, char **argv)
{
	char	*str;
	char	*new_argv;
	char	*writer;

	str = search_env("HOME", data->list_envs);
	if (!str)
		str = getenv("HOME");
	if (!str)
		return ;
	new_argv = malloc(ft_strlen(str) + ft_strlen(*argv));
	if (!new_argv)
		return ;
	writer = new_argv;
	while (*str)
		*(writer++) = *(str++);
	str = *argv + 1;
	while (*str)
		*(writer++) = *(str++);
	*writer = '\0';
	free(*argv);
	*argv = new_argv;
}
