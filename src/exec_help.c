/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_help.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:46:53 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 21:56:06 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// run a built-in command if it is one of the following:
char	run_builtin(t_data *data)
{
	if (!ft_strncmp(*data->argv, "echo", 5))
		data->status = builtin_echo(data->argv, data->argc);
	else if (!ft_strncmp(*data->argv, "cd", 3))
		data->status = builtin_cd(data);
	else if (!ft_strncmp(*data->argv, "pwd", 4))
		data->status = builtin_pwd();
	else if (!ft_strncmp(*data->argv, "export", 7))
		data->status = builtin_export(data->argv,
				data->argc, data->list_envs, data->export_list);
	else if (!ft_strncmp(*data->argv, "unset", 6))
		data->status = builtin_unset(data->argv, data->list_envs, NULL);
	else if (!ft_strncmp(*data->argv, "env", 4))
		data->status = builtin_env(data->list_envs);
	else if (!ft_strncmp(*data->argv, "exit", 5))
		data->status = builtin_exit(data->argv, data->argc, data);
	else
		return (0);
	return (1);
}

// creates an array of cmd-strings from a linked list of strings
// Also frees the linked_args list
char	create_argv_argc(t_data *data, t_list *linked_args)
{
	int		size;
	t_list	*tmp;
	char	**writer;

	size = ft_lstsize(linked_args);
	if (!size)
		return (0);
	data->argv = (char **)malloc((size + 1) * sizeof(char *));
	if (!data->argv)
		return (0);
	writer = data->argv;
	while (linked_args)
	{
		tmp = linked_args;
		clean_quotations(linked_args->content, linked_args->content, 0, 0);
		*(writer++) = linked_args->content;
		linked_args = linked_args->next;
		free(tmp);
	}
	*writer = NULL;
	data->argc = size;
	data->linked_args = NULL;
	return (1);
}

// remove list elements containing empty/no strings
t_list	*clean_args(t_list **args)
{
	t_list	*tmp;
	t_list	*prev;

	tmp = *args;
	prev = NULL;
	while (tmp)
	{
		if (!tmp->content || !*(char *)tmp->content)
		{
			if (prev)
				prev->next = tmp->next;
			else
				*args = tmp->next;
			free(tmp->content);
			free(tmp);
			tmp = prev;
		}
		prev = tmp;
		if (tmp)
			tmp = tmp->next;
	}
	return (*args);
}

// Also should handle "/bin/ls -laF" and "/bin/ls -l -a -F"
int	run_from_bin_path(t_data *data)
{
	char	*path_str;
	char	*filepath;
	char	file_exists;
	char	**tmp;

	path_str = search_env("PATH", data->list_envs);
	if (!path_str)
	{
		error_message(data->argv[0], NULL, "No such file or directory");
		return (127);
	}
	data->bin_paths = ft_split(path_str, ':');
	tmp = data->bin_paths;
	file_exists = 0;
	while (tmp && *tmp)
	{
		filepath = ft_strjoin(*tmp, "/");
		filepath = ft_strjoin(filepath, data->argv[0]);
		if (access(filepath, F_OK) == 0)
			file_exists = 1;
		if (access(filepath, X_OK) == 0)
			exit(ft_free(data, execve(filepath, data->argv, data->list_envs)));
		tmp++;
	}
	if (file_exists)
	{
		error_message(data->argv[0], NULL, "Permission denied");
		exit(ft_free(data, 126));
	}
	error_message(data->argv[0], NULL, "command not found");
	exit(ft_free(data, 127));
}
