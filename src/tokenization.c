/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:33 by bschneid          #+#    #+#             */
/*   Updated: 2024/09/11 14:31:38 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static size_t	count_tokens(char *str);
static	char	write_all_tokens(char *str, char **tokens_out, size_t tokens);
static size_t	is_operator(char *str);
char			clean_tokens(char **tokens);

// Allocates (with malloc(3)) and returns an array of token-strings 
// obtained by splitting on operators: ( ) | || && < << > >> . 
// The array ends with a NULL pointer.
char	**split_tokens(char *str)
{
	char	**tokens_out;
	size_t	tokens;

	if (!str)
		return (NULL);
	tokens = count_tokens(str);
	tokens_out = (char **)malloc((tokens + 1) * sizeof(char *));
	if (!tokens_out)
		return (NULL);
	if (!write_all_tokens(str, tokens_out, tokens))
		return (NULL);
	if (!clean_tokens(tokens_out))
	{
		ft_split_free(tokens_out);
		return (NULL);
	}
	return (tokens_out);
}

// returns 1 if >, >>, <, <<; else 0
char	is_redirection(char *str)
{
	if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1))
		return (1);
	else if (!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2))
		return (1);
	return (0);
}

char	*has_space(char *str)
{
	char	in_sgl;
	char	in_dbl;

	in_sgl = 0;
	in_dbl = 0;
	while (*str)
	{
		if (in_sgl || in_dbl)
		{
			if ((in_sgl && *str == '\'') || (in_dbl && *str == '"'))
			{
				in_sgl = 0;
				in_dbl = 0;
			}
		}
		else if (*str == '\'' || *str == '"')
		{
			if (*str == '\'')
				in_sgl = 1;
			else if (*str == '"')
				in_dbl = 1;
		}
		else if (*str == ' ')
			return (str);
		str++;
	}
	return (0);
}

//
char	clean_tokens(char **tokens)
{
	char	**writer;
	char	**checker;
	char	*copy;

	writer = tokens;
	while (*writer)
	{
		if (is_redirection(*writer) && has_space(*(writer + 1)))
		{
			checker = writer - 1;
			while (checker >= tokens)
			{
				if (!is_operator(*checker) && (checker == tokens || !is_redirection(*(checker - 1))))
				{
					copy = *checker;
					*checker = ft_strjoin(*checker, has_space(*(writer + 1)));
					if (!*checker)
						return (0);
					free(copy);
					*has_space(*(writer + 1)) = '\0';
					break ;
				}
				checker--;
			}
		}
		writer++;
	}
	return (1);
}

// 0 : no operator
// 1 : ( ) | < >
// 2 : && || << >>
static size_t	is_operator(char *str)
{
	if (!ft_strncmp(str, "&&", 2) || !ft_strncmp(str, "||", 2))
		return (2);
	else if (!ft_strncmp(str, ">>", 2) || !ft_strncmp(str, "<<", 2))
		return (2);
	else if (!ft_strncmp(str, "(", 1) || !ft_strncmp(str, ")", 1))
		return (1);
	else if (!ft_strncmp(str, ">", 1) || !ft_strncmp(str, "<", 1))
		return (1);
	else if (!ft_strncmp(str, "|", 1))
		return (1);
	return (0);		
}

static size_t	count_tokens(char *str)
{
	size_t	amount;
	char	in_single;
	char	in_double;
	
	amount = 0;
	in_single = 0;
	in_double = 0;
	while (*str)
	{
		if (*str == ' ')
			str++;
		else if (is_operator(str))				// cases with operators
		{
			amount++;
			str += is_operator(str);
		}
		else									// inside token-string
		{
			amount++;
			while (*str)
			{
				if (in_single || in_double)
				{
					if (in_single && *str == '\'')
						in_single = 0;
					else if (in_double && *str == '"')
						in_double = 0;
				}
				else if (*str == '\'')
					in_single = 1;
				else if (*str == '"')
					in_double = 1;
				else if (is_operator(str))
					break ;
				str++;
			}
		}
	}
	return (amount);
}

static	char	*token_write(char const *str, size_t len)
{
	char	*out;
	char	*writer;

	out = (char *)malloc((len + 1) * sizeof(char));
	if (!out)
		return (NULL);
	writer = out;
	while (len--)
		*(writer++) = *(str++);
	*writer = '\0';
	return (out);
}

static	char	write_all_tokens(char *str, char **tokens_out, size_t tokens)
{
	char	**writer;
	size_t	len;
	char	in_single;
	char	in_double;

	writer = tokens_out;
	while (tokens--)
	{
		while (*str == ' ')
			str++;
		if (is_operator(str))					// cases with operators
			len = is_operator(str);
		else									// inside token-string
		{
			len = 0;
			in_single = 0;
			in_double = 0;
			while (str[len])
			{
				if (in_single || in_double)
				{
					if (in_single && str[len] == '\'')
						in_single = 0;
					else if (in_double && str[len] == '"')
						in_double = 0;
				}
				else if (str[len] == '\'')
					in_single = 1;
				else if (str[len] == '"')
					in_double = 1;
				else if (is_operator(str + len))
					break ;
				len++;
			}
			while (str[len - 1] == ' ')
				len--;
		}
		*writer = token_write(str, len);
		if (!(*(writer++)))
		{
			ft_split_free(tokens_out);
			return (0);
		}
		str += len;
	}
	*writer = 0;
	return (1);
}
