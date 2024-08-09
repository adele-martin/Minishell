/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:24:16 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/09 19:54:51 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

static size_t	count_args(char *str);
static	char	write_all_args(char *str, char **args_out, size_t args);

char	**split_args(char *str)
{
	char	**args_out;
	size_t	args;

	if (!str)
		return (NULL);
	args = count_args(str);
	args_out = (char **)malloc((args + 1) * sizeof(char *));
	if (!args_out)
		return (NULL);
	if (!write_all_args(str, args_out, args))
		return (NULL);
	return (args_out);
}

static size_t	count_args(char *str)
{
	size_t	amount;
	char	in_single;
	char	in_double;
	
	amount = 1;
	in_single = 0;
	in_double = 0;
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
		else if (*str == ' ')
		{
			amount++;
			while (*(str + 1) == ' ')
				str++;
		}
		str++;
	}
	return (amount);
}

static	char	*arg_write(char const *str, size_t len)
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

static	char	write_all_args(char *str, char **args_out, size_t args)
{
	char	**writer;
	size_t	len;
	char	in_single;
	char	in_double;

	writer = args_out;
	while (args--)
	{
		len = 0;
		in_single = 0;
		in_double = 0;
		while (str[len])
		{
			if (str[len] == '\0')
				break ;
			else if (in_single || in_double)
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
			else if (str[len] == ' ')
				break ;
			len++;
		}
		*writer = arg_write(str, len);
		if (!(*(writer++)))
		{
			ft_split_free(args_out);
			return (0);
		}
		str += len;
		while (*str == ' ')
			str++;
	}
	*writer = 0;
	return (1);
}

// compile with:  cc src/split_cmd.c -L./src/Libft_extended -lft
// int	main(int argc, char **argv)
// {
// 	char	**args;
// 	size_t	i;

// 	args = split_args(argv[1]);
// 	i = 0;
// 	while (args[i])
// 	{
// 		printf("args[%lu]: %s\n", i, args[i]);
// 		i++;
// 	}
// 	ft_split_free(args);
// 	return (0);
// }
