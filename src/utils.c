/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 17:04:25 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/12 09:59:12 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

int with_value(char *string)
{
	int i = 0;
	while (string[i])
	{
		if (string[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	isalnum_space(int c)
{
	return (ft_isalpha(c) || ft_isdigit(c) || c == ' ');
}


int	isalpha_space(int c)
{
	return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == ' ');
}

char *ft_strcat(char* dest, const char* src)
{
	if (!dest || !src)
		return (NULL);
	char* ptr = dest;
	while (*ptr != '\0')
		ptr++;
	while (*src != '\0')
	{
		*ptr = *src;
		ptr++;
		src++;
	}
	*ptr = '\0';
	return dest;
}
