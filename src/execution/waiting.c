/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 16:28:47 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/17 16:31:23 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

// Adds a child pid to the list
int	fill_child_pid(t_pid **child_pids, pid_t id)
{
	t_pid	*new;

	while (*child_pids)
		child_pids = &(*child_pids)->next;
	new = (t_pid *)malloc(sizeof(t_list));
	if (!new)
		return (0);
	new->id = id;
	new->next = NULL;
	*child_pids = new;
	return (1);
}

// Waits for all children to finish
int	wait_for_children(t_pid *child_pids)
{
	int	status;

	while (child_pids)
	{
		waitpid(child_pids->id, &status, 0);
		child_pids = child_pids->next;
	}
	return (status);
}

