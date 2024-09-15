/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:45:17 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 20:53:27 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// Initializes all the struct data
int	initialize_data(t_data *data, int argc, char **envp)
{
	if (argc != 1)
		return (error_message(NULL, "main", "Too many arguments"), 0);
	data->list_envs = envs_list(envp);
	data->export_list = array_to_linkedlist(data->list_envs);
	data->shell_name = ft_strdup("minishell");
	data->status = 0;
	data->status_str = ft_itoa(0);
	data->input = NULL;
	data->tokens = NULL;
	data->ast_root = NULL;
	data->linked_args = NULL;
	data->id = 1;
	data->signal_fd = 0;
	data->in_child = 0;
	data->argc = 0;
	data->argv = NULL;
	data->linked_args = NULL;
	data->files_list = NULL;
	data->bin_paths = NULL;
	data->stdin = dup(STDIN_FILENO);
	data->stdout = dup(STDOUT_FILENO);
	handle_signals(0);
	return (1);
}

// Restore original STDIN and STDOUT
// option 0: only STDIN, 1: only STDOUT, 2: both
// Returns 1 on success, 0 on failure
int	restore_stdin_stdout(t_data *data, char option)
{
	if (option == 0 || option == 2)
		dup2(data->stdin, STDIN_FILENO);
	if (option == 1 || option == 2)
		dup2(data->stdout, STDOUT_FILENO);
	return (1);
}
