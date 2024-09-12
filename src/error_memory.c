/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 09:50:50 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/12 15:24:47 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

//Function to free array of strings
void	free_array(char ***array)
{
	size_t	i;

	if (!*array)
		return;
	i = 0;
	while (array[0][i])
		free(array[0][i++]);
	free(*array);
	*array = NULL;
}

void	error_message(char *cmd, char *arg, char *message)
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
	ft_putstr_fd(message, 2);
	ft_putstr_fd("\n", 2);
}

// TODO: Implement function to free the AST
int	ft_free(t_data *data, int exit)
{
	free(data->shell_name);
	if (data->list_envs)
		free_array(&data->list_envs);
	if (data->export_list)
		ft_lstclear(&data->export_list, free);
	if (data->status_str)
		free(data->status_str);
	if (data->input)
		free(data->input);
	if (data->tokens)
		free_array(&data->tokens);
	// if (data->astRoot) // TODO: Implement function to free the AST
	// 	free(data->astRoot);
	
	// if (data->linked_args)
	// 	ft_lstclear(&data->linked_args, free);	// TODO: CARE HOW TO FREE LINKED LIST

	close(data->stdin);
	close(data->stdout);
	rl_clear_history();
	return (exit);
}
