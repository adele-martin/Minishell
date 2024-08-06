/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/06 13:23:19 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// void	store_envs(char **envp)
// {
// 	char	**list_envs;
// 	int	i;

// 	list_envs = malloc(sizeof(char *) * 400);
// 	if (!list_envs)
// 		return (NULL);
// 	i = 0;
// 	while (envp[i])
// 	{
// 		list_envs[i] = malloc(strlen(envp[i]) + 1);
// 		ft_strlcpy(list_envs[i], envp[i], ft_strlen(envp[i]) + 1);
// 		i++;
// 	}
// 	// i = 0;
// 	// while (list_envs[i])
// 	// {
// 	// 	printf("%s\n", list_envs[i]);
// 	// 	i++;
// 	// }
// }

void store_envs(char **envp)
{
	EnvVars env_vars;
	int i = 0;

	env_vars.keys = malloc(sizeof(char *) * 400);
	env_vars.values = malloc(sizeof(char *) * 400);
	// if (!env_vars.keys || !env_vars.values)
	// 	return (EnvVars)NULL;

	while (envp[i])
	{
		char *env_str = envp[i];
		char *equal_sign = ft_strchr(env_str, '=');
		if (equal_sign != NULL)
		{
            size_t key_len = equal_sign - env_str;
            size_t value_len = ft_strlen(env_str) - key_len - 1;

            env_vars.keys[i] = malloc(key_len + 1);
            env_vars.values[i] = malloc(value_len + 1);

            // if (!env_vars.keys[i] || !env_vars.values[i])
			// {
            //     return (EnvVars){NULL, NULL};
            // }

            ft_strncpy(env_vars.keys[i], env_str, key_len);
            env_vars.keys[i][key_len] = '\0';

            ft_strncpy(env_vars.values[i], equal_sign + 1, value_len);
            env_vars.values[i][value_len] = '\0';
        }
        i++;
	}
	i = 0;
	while (env_vars.keys[i])
	{
		printf("%s\n", env_vars.keys[i]);
		i++;
	}
	// return (env_vars);
}



//All built­in functions except printenv are executed by the parent process
//Figure out how to deal with the quotes