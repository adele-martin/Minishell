/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/07 12:19:23 by ademarti         ###   ########.fr       */
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
		ft_strlcpy(list_envs[i], envp[i], ft_strlen(envp[i]) + 1);
		i++;
	}
	return (list_envs);
}

// variabe is key=value
char	**variables_list(char *variable, char **list_envs)
{
	int	i;
	i = 0;
	int j = 0;

	while (list_envs[i])
	{
		j = 0;
		while (list_envs[i][j] != '=')
			j++;
		if (ft_strncmp(list_envs[i], variable, j) == 0)
		{
			list_envs[i] = (char *)malloc(sizeof(char) * ft_strlen(variable) + 1);
			list_envs[i] = variable;
			break;
		}
		i++;
	}
	return (list_envs);
}

// void update_envs(char *input, char **list_envs)
// {
// 	int i;
// 	int j;
// 	int input_len;

// 	i = 0;
// 	j = 0;
// 	input_len = ft_strlen(input);
// 	while (list_envs[i])
// 	{
// 		while (list_envs[i][j])
// 		{
// 			if (ft_strncmp(list_envs[i], input, input_len) == 0)
// 			j++;
// 		}
// 	}
// }



//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes