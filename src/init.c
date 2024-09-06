/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 12:45:17 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/04 15:54:57 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// for getting key-value: char	*expanding(char *variable, char **list, t_list *head)
int	initialize_data(t_data *data, int argc, char **argv, char **envp)
{
	(void)argv;
	if (argc != 1)
	{
		errno = EINVAL;
		perror("main");
		exit (EXIT_FAILURE);
	}
	data->list_envs = envs_list(envp);
	data->export_list = arrayToLinkedList(data->list_envs);
	data->tty_name = ttyname(STDIN_FILENO);
	data->shell_name = ft_strdup("minishell");
	data->status_str = ft_itoa(123);
	data->in_pipe = 0;
	data->id = 1;
	data->signal_fd = 0;
	// data->cmd_argc = 0;
	data->exit = 0;
	// ft_printf("TEST-EXPAND: %s\n", search("PWDasdf", data->list_envs, NULL));
	handle_signals();
	return (EXIT_SUCCESS);
}

// Restore original STDIN and STDOUT
// option 0: only STDIN, 1: only STDOUT, 2: both
// Returns 1 on success, 0 on failure
int	restore_stdin_stdout(t_data *data, char option)
{
	int	tty_fd;

	tty_fd = open(data->tty_name, O_RDWR);
	if (tty_fd == -1) {
		perror("open terminal");
		return (0);
	}
	if (option == 0 || option == 2)
		dup2(tty_fd, STDIN_FILENO);  // Restore STDIN
	if (option == 1 || option == 2)
		dup2(tty_fd, STDOUT_FILENO); // Restore STDOUT
	close(tty_fd);
	return (1);
}
