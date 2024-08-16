/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/16 17:14:40 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/16 17:14:42 by bschneid         ###   ########.fr       */
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
	if (sig == SIGINT)
		ft_printf("\nPressed CTRL-C\n");
	else if (sig == SIGQUIT)
		ft_printf("\nPressed CTRL-\\\n");
}

void	handle_signals(void)
{
	struct sigaction	action;

	sigemptyset(&action.sa_mask);
	sigaddset(&action.sa_mask, SIGINT);		// ctrl-C
	sigaddset(&action.sa_mask, SIGQUIT);	/* ctrl-\ */ 
	action.sa_flags = SA_RESTART;
	action.sa_handler = signal_action;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
	
	// return (0);
}
