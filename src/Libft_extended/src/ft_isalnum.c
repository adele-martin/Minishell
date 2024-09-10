/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isalnum.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <adelemartin@student.42.fr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:56:23 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/10 11:24:32 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/libft.h"

int	ft_isalnum(int c)
{
	return ((unsigned char)ft_isalpha(c) || (unsigned char)ft_isdigit(c));
}
