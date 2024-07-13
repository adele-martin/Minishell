/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isai.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 15:38:58 by bschneid          #+#    #+#             */
/*   Updated: 2024/05/27 15:41:37 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

char	ft_isai(const char *string)
{
	if (*string == '-' || *string == '+')
		string++;
	if (!*string)
		return (0);
	while (*string)
	{
		if (*string < '0' || *string > '9')
			return (0);
		string++;
	}
	return (1);
}
