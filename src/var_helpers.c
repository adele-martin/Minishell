/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   var_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 18:39:34 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/13 18:39:36 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static size_t	len_after_replace(char *original, t_vars *vars);

char	replace_vars(t_list *arg, t_vars *vars)
{
	char	*new;
	char	*old_ptr;
	char	*cpy;

	new = (char *)malloc(len_after_replace(arg->content, vars) * sizeof(char));
	if (!new)
		return (0);
	cpy = new;
	old_ptr = arg->content;
	while (*old_ptr)
	{
		if (vars && old_ptr == vars->key_start)
		{
			if (vars->value_start)
				while (*vars->value_start)
					*(cpy++) = *(vars->value_start++);
			old_ptr += vars->key_len;
			vars = vars->next;
		}
		else
			*(cpy++) = *(old_ptr++);
	}
	*cpy = '\0';
	free(arg->content);
	arg->content = new;
	return (1);
}

static size_t	len_after_replace(char *original, t_vars *vars)
{
	size_t	len;

	len = ft_strlen(original);
	while (vars)
	{
		len += vars->len_diff;
		vars = vars->next;
	}
	return (len + 1);
}
