/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 09:50:50 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/12 09:52:09 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

//Function to free array of strings
void freeArray(char **array)
{
    if (!array)
        return;
    int i = 0;
    while (array[i] != NULL)
    {
        free(array[i]);
        i++;
    }
    free(array);
}

//Function to free linked list.
void freeList(t_list *head)
{
    t_list *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;

        free(temp->content);
        free(temp);
    }
}

void error_message(char *cmd, char *arg, char *message)
{
	ft_putstr_fd("minishell: ", 2);
	if (cmd)
	{
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (arg)
	{
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": ", 2);
	}
}

int ft_free(t_data *data, int exit)
{
	if (data->list_envs)
		freeArray(data->list_envs);
	if (data->export_list)
		freeList(data->export_list);
    if (data->shell_name)
        free(data->shell_name);
    if (data->status_str)
        free(data->status_str);
    if (data->input)
        free(data->input);
    rl_clear_history();
    return (exit);
}
