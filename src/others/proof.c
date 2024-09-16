/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proof.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 11:40:18 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/16 23:12:12 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minishell.h"

static void	spec_error(char *token);
static char	valid_string(char *str, char in_sgl, char in_dbl);

// needs_string: 0 = no, 1 = yes (string must follow !)
char	check_input(char *str, char needs_string)
{	// start mit 0 (kein String muss folgen)
	while (*str == ' ')
		str++;
	if (!*str && !needs_string)
		return (1);
	else if (!*str)
		return (spec_error("newline"), 0);
	
	if (!ft_strncmp(str, "&&", 2))
		return (spec_error("&&"), 0);
	if (!ft_strncmp(str, "||", 2))
		return (spec_error("||"), 0);
	if (!ft_strncmp(str, "&", 1))
		return (spec_error("&"), 0);
	if (!ft_strncmp(str, "|&", 2))
		return (spec_error("|&"), 0);
	if (!ft_strncmp(str, "|", 1))
		return (spec_error("|"), 0);
	
	if (!needs_string && (!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2)))
		return (check_input(str + 2, 1));
	if (!ft_strncmp(str, ">>", 2))
		return (spec_error(">>"), 0);
	if (!ft_strncmp(str, "<<", 2))
		return (spec_error("<<"), 0);
	
	if (!needs_string && (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1)))
		return (check_input(str + 1, 1));
	if (!ft_strncmp(str, ">", 1))
		return (spec_error(">"), 0);
	if (!ft_strncmp(str, "<", 1))
		return (spec_error("<"), 0);
	
	return (valid_string(str, 0, 0));
}

static void	spec_error(char *token)
{
	error_message("syntax error", "unexpected token", token);
}

static char	valid_string(char *str, char in_sgl, char in_dbl)
{
	if (*str == '&' || *str == '|' || *str == '>' || *str == '<')
		return (0);
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
		else if (!ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2) ||
					!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2))
			return (check_input(str + 2, 1));
		else if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1))
			return (check_input(str + 1, 1));
		else if (!ft_strncmp(str, "|", 1))
		{
			str++;
			while (*str == ' ')
				str++;
			if (!*str)
				return (error_message(NULL, "syntax error", "pipe at end of input"), 0);
			return (check_input(str, 0));
		}
		else if (!ft_strncmp(str, "(", 1))
			return (check_input(str + 1, 0));
		else if (!ft_strncmp(str, ")", 1))
			return (check_input(str + 1, 0));
		str++;
	}
	if (in_sgl || in_dbl)
		return (error_message(NULL, NULL, "syntax error: unclosed quotation mark"), 0);
	return (1);
}
