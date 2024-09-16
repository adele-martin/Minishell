/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proof.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:40:18 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/16 11:54:14 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

char	check_input(char *input, char in_sgl, char in_dbl)
{
	while (*str)
	{
		if (in_sgl || in_dbl)
		{
			if ((in_sgl && *str == '\'') || (in_dbl && *str == '"'))
			{
				in_sgl = 0;
				in_dbl = 0;
				str++;
				continue ;
			}
		}
		else if (*str == '\'' || *str == '"')
		{
			if (*str == '\'')
				in_sgl = 1;
			else if (*str == '"')
				in_dbl = 1;
			str++;
			continue ;
		}
		*(writer++) = *(str++);
	}
	return (1);

	if (!input)
		return (0);
	if (ft_strlen(input) == 0)
	{
		free(input);
		return (0);
	}
	return (1);
}

// 0 : no operator
// 1 : ( ) | < >
// 2 : && || << >>
// char	is_operator(char *str)
// {
// 	if (!ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2))
// 		return (2);
// 	else if (!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2))
// 		return (2);
// 	else if (!ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1))
// 		return (1);
// 	else if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1))
// 		return (1);
// 	else if (!ft_strncmp(str, "|", 1))
// 		return (1);
// 	return (0);
// }
