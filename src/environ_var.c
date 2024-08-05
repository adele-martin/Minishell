/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/05 17:56:06 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

store_envs(char **envp)
{
	char **list_envs;
	list_envs = malloc(sizeof(char *) * 400);

	int	i;
	i = 0;
	while (envp[i])
	{
		ft_strcpy(list_envs[i], envp[i]);
		i++;
	}
	i = 0;
	while (list_envs[i])
	{
		printf("%s\n", list_envs);
		i++;
	}
}

//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes