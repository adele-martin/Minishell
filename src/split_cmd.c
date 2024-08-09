/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 17:24:16 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/09 22:33:36 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

t_list	*get_args(char *str)
{
	t_list	*args_out;
	t_list	**writer;
	char	*tmp;
	size_t	len;
	char	in_single;
	char	in_double;

	if (!str)
		return (NULL);
	writer = &args_out;
	while (*str)
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
		tmp = ft_substr(str, 0, len);
		if (!tmp)
		{
			ft_lstclear(&args_out, free);
			return (NULL);
		}
		*writer = ft_lstnew(tmp);
		if (!*writer)
		{
			ft_lstclear(&args_out, free);
			return (NULL);
		}
		writer = &(*writer)->next;
		str += len;
		while (*str == ' ')
			str++;
	}
	*writer = 0;
	return (args_out);
}

// compile with:  cc src/split_cmd.c -L./src/Libft_extended -lft
int	main(int argc, char **argv)
{
	t_list	*args;

	args = get_args(argv[1]);
	while (args)
	{
		printf("args: %s\n", (char *)args->content);
		args = args->next;
	}
	return (0);
}

/*
1. Expand $ variables in every list element in normal string and in double quotes
	- In this step, every inserted * will be filled as '\*'
	- Every $ will be expanded to the value of the variable
	- If a variable is not set, it will be expanded to an empty string
	- all quotes will be removed from the string (no escaping)
2. Expand * wildcards in every list element (add more elements if necessary)
	- a '\*' will be expanded to the char '*'
	- a '*' will be a wildcard for any string
3. Clear every '\*' from the string
*/
