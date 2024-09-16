/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 20:43:06 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/16 09:33:15 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

//This function creates the initial env array of strings from the main.
char	**envs_list(char **envp)
{
	char	**list_envs;
	int		i;

	list_envs = malloc(sizeof(char *) * 400);
	if (!list_envs)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		list_envs[i] = ft_strdup(envp[i]);
		if (!list_envs[i])
			return (NULL);
		i++;
	}
	while (i < 400)
	{
		list_envs[i] = NULL;
		i++;
	}
	return (list_envs);
}
