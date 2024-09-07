/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/12 18:11:03 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/07 16:43:20 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static int		run_from_bin_path(t_data *data);
char	**create_argv(t_list *linked_args);

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

// TODO: else if (is_buildin(cmd_argv[0]))
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
		return (perror("Error in wildcards"), 1);
	if (!expand_variables(data->linked_args, data))
		return (perror("Error in expanding variables"), 1);
	data->cmd_argv = create_argv(data->linked_args);
	if (!data->cmd_argv)
		return (1);
	char	**argv_tmp = data->cmd_argv;
	char	cmd_argc = 0;
	while (*(argv_tmp++))
		cmd_argc++;
	// possibilities for executions:
	if (ft_strncmp(*data->cmd_argv, "echo", 5) == 0)
		builtin_echo(data->cmd_argv, cmd_argc);
	else if (ft_strncmp(*data->cmd_argv, "cd", 3) == 0)
		builtin_cd(data->cmd_argv, cmd_argc, data->list_envs);
	else if (ft_strncmp(*data->cmd_argv, "pwd", 4) == 0)
		builtin_pwd();
	else if (ft_strncmp(*data->cmd_argv, "export", 7) == 0)
		builtin_export(data->cmd_argv, cmd_argc, data->list_envs, data->export_list);
	else if (ft_strncmp(*data->cmd_argv, "unset", 6) == 0)
		builtin_unset(data->cmd_argv, data->list_envs, NULL);
	else if (ft_strncmp(*data->cmd_argv, "env", 4) == 0)
		builtin_env(data->list_envs);
	else if (ft_strncmp(*data->cmd_argv, "exit", 5) == 0)
		builtin_exit(data->cmd_argv, cmd_argc);
	else
	{
		data->id = fork();
		if (data->id == -1)
			return (1);
		if (data->id == 0)
		{
			if (**data->cmd_argv == '.')
			{
				if (access(data->cmd_argv[0], X_OK) == 0)
					execve(data->cmd_argv[0], data->cmd_argv, data->list_envs);
				ft_printf("minishell: %s: Permission denied\n", data->cmd_argv[0]);
				exit (126);
			}
			else
				run_from_bin_path(data);
		}
		else
		{
			waitpid(data->id, &data->status, 0);
			if (WIFEXITED(data->status))
				data->status = WEXITSTATUS(data->status);
		}
	}
	// builtin_env(data->cmd_argv, data->cmd_argc, data->list_envs);
	// builtin_pwd(data->cmd_argv, data->list_envs);
	if (data->id)
		return (0);
	exit (data->status);
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

static int	run_from_bin_path(t_data *data)
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
		filepath = ft_strjoin(filepath, data->cmd_argv[0]);
		if (access(filepath, F_OK) == 0)
			file_exists = 1;
		if (access(filepath, X_OK) == 0)
		{
			execve(filepath, data->cmd_argv, data->list_envs);
		}
		bin_paths++;
	}
	if (file_exists)
	{
		ft_printf("minishell: %s: Permission denied\n", data->cmd_argv[0]);
		exit (126);
	}
	ft_printf("%s: command not found\n", data->cmd_argv[0]);
	exit (127);
}

// TODO: Exit statuses: https://www.redhat.com/sysadmin/exit-codes-demystified
