/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:14:40 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/04 17:23:08 by bschneid         ###   ########.fr       */
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

void	signal_action(int sig)
{
	(void)sig;
	ft_printf("\n");
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
	g_signal = 130;
}

// TODO: different signal handling for different spots in main! And maybe for heredoc too!
/* SIGINT: ctrl-C; SIGQUIT: ctrl-\ */
void	handle_signals(char option)
{
	static struct sigaction	action;
	static struct sigaction	ignore;

	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGINT);
	action.sa_flags = SA_RESTART;
	if (option == 1)
		action.sa_handler = SIG_IGN;
	else
	action.sa_handler = signal_action;
	sigaction(SIGINT, &action, NULL);

	sigemptyset(&ignore.sa_mask);
	sigaddset(&ignore.sa_mask, SIGQUIT);
	ignore.sa_flags = SA_RESTART;
	ignore.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &ignore, NULL);
}
