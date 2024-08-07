/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/07 15:47:38 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

char	**envs_list(char **envp)
{
	char	**list_envs;
	int	i;

	list_envs = malloc(sizeof(char *) * 400);
	if (!list_envs)
		return (NULL);
	i = 0;
	while (envp[i])
	{
		list_envs[i] = malloc(strlen(envp[i]) + 1);
		// ft_strlcpy(list_envs[i], envp[i], ft_strlen(envp[i]) + 1);
		// free(list[i]);
		list_envs[i] = ft_strdup(envp[i]);
		i++;
	}
	return (list_envs);
}

// char	**update_list(char *variable, char **list)
// {
// 	int	i;

// 	i = 0;
// 	str_len()
// 	while (list[i])
// 	{
// 		j = 0;
// 		while (list[i][j] != '=')
// 			j++;
// 		if (ft_strncmp(list[i], variable, j) == 0)
// 		{
// 			found = 1;
// 			free(list[i]);
// 			list[i] = (char *)malloc(sizeof(char) * ft_strlen(variable) + 1);
// 			list_envs[i] = ft_strdup(variable);
// 			break;
// 		}
// 		i++;
// 	}
// 	if (!found)
// 		append_to_list();
// 	return (list);
// }

// variable is key=value
char	**update_list(char *variable, char **list)
{
	int	i;
	int	j;
	int found;

	j = 0;
	i = 0;
	found = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (ft_strncmp(list[i], variable, j) == 0)
		{
			found = 1;
			free(list[i]);
			list[i] = ft_strdup(variable);
			break;
		}
		i++;
	}
	// if (!found)
	// 	append_to_list(list);
	return (list);
}