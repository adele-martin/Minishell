/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:42:33 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/19 14:11:34 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

int	builtin_unset(t_data *data)
{
	int	i;

	i = 1;
	while (data->argv[i])
	{
		delete_env(data->argv[i], data->list_envs);
		i++;
	}
	i = 1;
	while (data->argv[i])
	{
		if (return_value_var(data->argv[i], data->export_list))
			delete_var(data->argv[i], data);
		i++;
	}
	return (0);
}
