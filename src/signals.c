/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:14:40 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 17:59:13 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

/*
• Handle ctrl-C, ctrl-D and ctrl-\ which should behave like in bash.
• In interactive mode:
◦ ctrl-C displays a new prompt on a new line.
◦ ctrl-D exits the shell.
◦ ctrl-\ does nothing.

use of kill is allowed
*/

void	signal_in_newline(int sig)
{
	g_signal = 130;
	write(2, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	(void)sig;
}

void	signal_out_newline(int sig)
{
	g_signal = 130;
	write(2, "\n", 1);
	(void)sig;
}

// TODO: Add exit correction here - signal in HEREDOC !!
// Handles signals in child processes like in heredocs or piping
void	signal_child(int sig)
{
	(void)sig;
	g_signal = 130;
	// rl_clear_history();
	// ioctl(STDIN_FILENO, TIOCSTI, "\n");
	exit (130);
	// ft_printf("EXIT IN CHILD\n");
}
// void	signal_heredoc(int sig)
// {
// 	ioctl(STDIN_FILENO, TIOCSTI, "\n");
// }

//Add exit correction here
// TODO: different signal handling for different spots in main! And maybe for heredoc too!
// option 0: initialize: ignore SIGINT, handle SIGQUIT
// option 1: set SIGINT to signal_in_newline
// option 2: set SIGINT to signal_out_newline
// option 3: set SIGINT to signal_child
/* SIGINT: ctrl-C; SIGQUIT: ctrl-\ */
void	handle_signals(char option)
{
	static struct sigaction	action;
	static struct sigaction	ignore;

	if (option == 0)
	{
		sigemptyset(&action.sa_mask);
		sigaddset(&action.sa_mask, SIGINT);
		action.sa_flags = SA_RESTART;
		action.sa_handler = signal_in_newline;
		sigemptyset(&ignore.sa_mask);
		sigaddset(&ignore.sa_mask, SIGQUIT);
		ignore.sa_flags = SA_RESTART;
		ignore.sa_handler = SIG_IGN;
		sigaction(SIGQUIT, &ignore, NULL);
	}
	else if (option == 1)
		action.sa_handler = signal_in_newline;
	else if (option == 2)
		action.sa_handler = signal_out_newline;
	else if (option == 3)
		action.sa_handler = signal_child;
	sigaction(SIGINT, &action, NULL);
}
