/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   evaluation_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:46:53 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 19:36:02 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// run a built-in command if it is one of the following:
char	run_builtin(t_data *data)
{
	if (!ft_strncmp(*data->cmd_argv, "echo", 5))
		data->status = builtin_echo(data->cmd_argv, data->cmd_argc);
	else if (!ft_strncmp(*data->cmd_argv, "cd", 3))
		data->status = builtin_cd(data);
	else if (!ft_strncmp(*data->cmd_argv, "pwd", 4))
		data->status = builtin_pwd();
	else if (!ft_strncmp(*data->cmd_argv, "export", 7))
		data->status = builtin_export(data->cmd_argv, 
				data->cmd_argc, data->list_envs, data->export_list);
	else if (!ft_strncmp(*data->cmd_argv, "unset", 6))
		data->status = builtin_unset(data->cmd_argv, data->list_envs, NULL);
	else if (!ft_strncmp(*data->cmd_argv, "env", 4))
		data->status = builtin_env(data->list_envs);
	else if (!ft_strncmp(*data->cmd_argv, "exit", 5))
		data->status = builtin_exit(data->cmd_argv, data->cmd_argc, data);
	else
		return (0);
	return (1);
}

// creates an array of cmd-strings from a linked list of strings
char	**create_argv(t_list *linked_args)
{
	int		size;
	t_list	*tmp;
	char	**out;
	char	**writer;

	size = ft_lstsize(linked_args);
	if (!size)
		return (NULL);
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

int	get_argc(char **argv)
{
	int	argc;

	argc = 0;
	while (*(argv++))
		argc++;
	return (argc);
}

// remove list elements containing empty/no strings
void	clean_args(t_list **args)
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
}
