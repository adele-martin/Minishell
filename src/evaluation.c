/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/12 11:15:20 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static int	get_argc(char **argv);
static int	run_from_bin_path(t_data *data);
char		**create_argv(t_list *linked_args);

/*
The builtins should not be executed in the child process, but in the parent process:
◦ echo with option -n
◦ cd with only a relative or absolute path
◦ pwd with no options
◦ export with no options
◦ unset with no options
◦ env with no options or arguments
◦ exit with no options
void builtin_export(char **argv, int argc, char **list_envs);
void	builtin_echo(char **argv, int argc);
void builtin_env(char **argv, int argc, char **list_envs);
void builtin_unset(char **argv, char **list_envs);
void builtin_cd(char **argv, int argc);
void builtin_pwd(char **argv, int argc);
void builtin_exit(char **argv, int argc);
*/

// 	return (run_buildin(cmd_argv));
int	execute(char *input, t_data *data)
{
	// ft_putstr_fd(data->in_child, 2);
	// ft_putstr_fd(input, 2);
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
	data->cmd_argv = create_argv(data->linked_args);
	if (!data->cmd_argv)
		return (1);
	data->cmd_argc = get_argc(data->cmd_argv);
	// possibilities for executions:
	if (ft_strncmp(*data->cmd_argv, "echo", 5) == 0)
		data->status = builtin_echo(data->cmd_argv, data->cmd_argc);
	else if (ft_strncmp(*data->cmd_argv, "cd", 3) == 0)
		data->status = builtin_cd(data);
	else if (ft_strncmp(*data->cmd_argv, "pwd", 4) == 0)
		data->status = builtin_pwd();
	else if (ft_strncmp(*data->cmd_argv, "export", 7) == 0)
		data->status = builtin_export(data->cmd_argv, data->cmd_argc, data->list_envs, data->export_list);
	else if (ft_strncmp(*data->cmd_argv, "unset", 6) == 0)
		data->status = builtin_unset(data->cmd_argv, data->list_envs, NULL);
	else if (ft_strncmp(*data->cmd_argv, "env", 4) == 0)
		data->status = builtin_env(data->list_envs);
	else if (ft_strncmp(*data->cmd_argv, "exit", 5) == 0)
		data->status = builtin_exit(data->cmd_argv, data->cmd_argc, data);
	else
	{
		if (!data->in_child)
			data->id = fork();
		if (data->id == -1)
			return (1);
		if (data->in_child || data->id == 0)
		{
			if (**data->cmd_argv == '.' || **data->cmd_argv == '/' || **data->cmd_argv == '~')
			{
				if (ft_strncmp(*data->cmd_argv, "~/", 2) == 0)
					update_home(data, data->cmd_argv);
				if (access(data->cmd_argv[0], X_OK) == 0)
					exit(ft_free(data, execve(data->cmd_argv[0], data->cmd_argv, data->list_envs)));
				error_message(data->cmd_argv[0], NULL, strerror(errno));
				exit (ft_free(data, 126));
			}
			else
				exit(ft_free(data, run_from_bin_path(data)));
		}
		// waitpid(data->id, &data->status, 0);
		// if (WIFEXITED(data->status))
		// 	data->status = WEXITSTATUS(data->status);
	}
	if (!data->in_child)
	// TODO: Care if a builtin was run and handle wait differently?!
	{
		waitpid(data->id, &data->status, 0);
		// if (WIFSIGNALED(data->status))
		// 	data->status = 128 + WTERMSIG(data->status);
		if (WIFEXITED(data->status))
			data->status = WEXITSTATUS(data->status);
		// ft_printf("exit status: %d\n", data->status);
		return (data->status);
	}
	exit (ft_free(data, data->status));
}

static int	get_argc(char **argv)
{
	int	argc;

	argc = 0;
	while (*(argv++))
		argc++;
	return (argc);
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
		error_message(NULL, data->cmd_argv[0], ": No such file or directory\n");
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
		error_message(NULL, data->cmd_argv[0], ": Permission denied\n");
		exit (ft_free(data, 126));
	}
	error_message(NULL, data->cmd_argv[0], ": command not found\n");
	exit(ft_free(data, 127));
}

// TODO: Exit statuses: https://www.redhat.com/sysadmin/exit-codes-demystified
