/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:45:17 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 18:31:35 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// for getting key-value: char	*expanding(char *variable, char **list, t_list *head)
int	initialize_data(t_data *data, int argc, char **envp)
{
	if (argc != 1)
	{
		error_message(NULL, "main", "Too many arguments");
		exit(EXIT_FAILURE);
	}
	data->list_envs = envs_list(envp);
	data->export_list = array_to_linkedlist(data->list_envs);
	data->shell_name = ft_strdup("minishell");
	data->status = 0;
	data->status_str = ft_itoa(0);
	data->input = NULL;
	data->tokens = NULL;
	data->ast_root = NULL;
	data->linked_args = NULL;
	data->in_pipe = 0;
	data->id = 1;
	data->signal_fd = 0;
	data->in_child = 0;
	data->cmd_argc = 0;
	data->cmd_argv = NULL;
	data->stdin = dup(STDIN_FILENO);
	data->stdout = dup(STDOUT_FILENO);
	handle_signals(0);
	return (EXIT_SUCCESS);
}

// Restore original STDIN and STDOUT
// option 0: only STDIN, 1: only STDOUT, 2: both
// Returns 1 on success, 0 on failure
int	restore_stdin_stdout(t_data *data, char option)
{
	if (option == 0 || option == 2)
		dup2(data->stdin, STDIN_FILENO);  // Restore STDIN
	if (option == 1 || option == 2)
		dup2(data->stdout, STDOUT_FILENO); // Restore STDOUT
	return (1);
}
