/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/09 16:14:47 by ademarti         ###   ########.fr       */
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
		list_envs[i] = ft_strdup(envp[i]);
		if (!list_envs[i])
			return NULL;
		i++;
	}
	while (i < 400)
	{
		list_envs[i] = NULL;
		i++;
	}
	return (list_envs);
}

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
			if (!list[i])
				return NULL; //Add a shield function
			break;
		}
		i++;
	}
	if (!found)
		list[i] = ft_strdup(variable);
	return (list);
}

char	**delete_var(char *variable, char **list)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (list[i])
	{
		j = 0;
		while (list[i][j] != '=')
			j++;
		if (ft_strncmp(list[i], variable, j) == 0)
		{
			free(list[i]);
			while (list[i + 1])
			{
				list[i] = list[i + 1];
				i++;
			}
			list[i] = NULL;
			break;
		}
		i++;
	}
	return (list);
}

// void expand_variable(char *string)
// {
// while (argv[i][j])
// 		{
// 			if (argv[i][j] == '$')
// 			{
// 				while (argv[i][j] != ' ')
// 				{
// 					variable[j] = argv[i][j];
// 					j++;
// 				}
// 			}
// }

	// char **list_envs = envs_list(envp);
	// list_envs = update_list("chicken=soup", list_envs);

	// int i = 0;
	// while (list_envs[i])
	// {
	// 	printf("%s\n", list_envs[i]);
	// 	i++;
	// }