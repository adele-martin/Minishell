/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 19:20:15 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static int	run_extern(t_data *data);
int			action_in_child(t_data *data);
static int	run_from_bin_path(t_data *data);

// just for debugging
// static void	print_argv(char **argv)
// {
// 	int	i;

// 	i = 0;
// 	while (argv[i])
// 		ft_printf("ARG: -->%s<--\n", argv[i++]);
// }

// 	return (run_buildin(cmd_argv));
int	execute(char *input, t_data *data)
{
	if (data->signal_fd)
	{
		write(data->signal_fd, &data->in_pipe, sizeof(char));
		close(data->signal_fd);
	}
	if (!input || !*input)
		return (0);
	data->linked_args = get_args(input);
	if (!add_wildcards(data->linked_args))
	{
		ft_lstclear(&data->linked_args, free);
		return (error_message(NULL, NULL, "Error in wildcards"), 1);
	}
	if (!expand_variables(data->linked_args, data))
	{
		ft_lstclear(&data->linked_args, free);
		return (error_message(NULL, NULL, "Error in expanding variables"), 1);
	}
	clean_args(&data->linked_args);
	if (!data->linked_args)
		return (0);
	data->cmd_argv = create_argv(data->linked_args);
	if (!data->cmd_argv)
		return (1);
	data->cmd_argc = get_argc(data->cmd_argv);
	if (!run_builtin(data))					// run builtin if given
		data->status = run_extern(data);	// run external command
	else if (!data->in_child)		// if builtin was run in child, don't wait for it
		return (data->status);
	waitpid(data->id, &data->status, 0);
	if (WIFEXITED(data->status))
		data->status = WEXITSTATUS(data->status);
	return (data->status);
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

int	action_in_child(t_data *data)
{
	struct stat	path_stat;
	
	if (ft_strncmp(data->cmd_argv[0], "~/", 2) == 0)
		update_home(data, data->cmd_argv);
	if (**data->cmd_argv == '.' || **data->cmd_argv == '/')
	{
		if (stat(data->cmd_argv[0], &path_stat) != 0)
			return(error_message(data->cmd_argv[0], NULL, "No such file or directory"), 127);
		if (S_ISDIR(path_stat.st_mode))
			return(error_message(data->cmd_argv[0], NULL, "Is a directory"), 126);
		
		if (access(data->cmd_argv[0], X_OK) == 0)
			exit(ft_free(data, execve(data->cmd_argv[0], data->cmd_argv, data->list_envs)));
		return (error_message(data->cmd_argv[0], NULL, "Permission denied"), 126);
	}
	else
		return(ft_free(data, run_from_bin_path(data)));
	return (0);
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


// TODO: Can also begin with the directory like "/bin/ls"
// Also should handle "/bin/ls -laF" and "/bin/ls -l -a -F"
static int	run_from_bin_path(t_data *data)
{
	char	*path_str;
	char	**bin_paths;
	char	*filepath;
	char	file_exists;

	path_str = search_env("PATH", data->list_envs);
	if (!path_str)
	{
		error_message(data->cmd_argv[0], NULL, "No such file or directory");
		return(127);
	}
	bin_paths = ft_split(path_str, ':');
	file_exists = 0;
	while (*bin_paths)
	{
		filepath = ft_strjoin(*bin_paths, "/");
		filepath = ft_strjoin(filepath, data->cmd_argv[0]);
		if (access(filepath, F_OK) == 0)
			file_exists = 1;
		if (access(filepath, X_OK) == 0)
			exit(ft_free(data, execve(filepath, data->cmd_argv, data->list_envs)));
		bin_paths++;
	}
	if (file_exists)
	{
		error_message(data->cmd_argv[0], NULL, "Permission denied");	
		exit (ft_free(data, 126));
	}
	error_message(data->cmd_argv[0], NULL, "command not found");
	exit(ft_free(data, 127));
}

// TODO: Exit statuses: https://www.redhat.com/sysadmin/exit-codes-demystified
