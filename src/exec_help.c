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
