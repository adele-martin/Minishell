/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   built-ins.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ademarti <ademarti@student.42berlin.de     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/03 16:21:26 by ademarti          #+#    #+#             */
/*   Updated: 2024/08/03 17:42:22 by ademarti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//All built­in functions except printenv are executed by the parent process
//Should I have an overview built-in command?
//How does the parsing work?
int	echo_builtin(char *args)
{
	char *line;

	return (0);
}