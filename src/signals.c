/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:14:40 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 21:00:10 by bschneid         ###   ########.fr       */
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
	ft_printf("\nPressed CTRL-C\nminishell > ");
}

/* SIGINT: ctrl-C; SIGQUIT: ctrl-\ */
void	handle_signals(void)
{
	struct sigaction	action;
	struct sigaction	ignore;

	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGINT);
	action.sa_flags = SA_RESTART;
	action.sa_handler = signal_action;
	sigaction(SIGINT, &action, NULL);

	sigemptyset(&ignore.sa_mask);
	sigaddset(&ignore.sa_mask, SIGQUIT);
	ignore.sa_flags = SA_RESTART;
	ignore.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &ignore, NULL);
}
