/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:24 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/14 13:42:03 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// * (asterisk) this can represent any number of characters (including zero, 
	// in other words, zero or more characters). If you specified a "cd*" it 
	// would use "cda", "cdrom", "cdrecord" and anything that starts with “cd”
	// also including “cd” itself. "m*l" could by mill, mull, ml, and anything 
	// that starts with an m and ends with an l.

TODO:
char	add_wildcards(t_list **linked_args)
{
	t_list	*files_list;
	t_list	*tmp;
	t_list	**writer;

	files_list = get_files_list();
	if (!files_list)
		return (0);
	writer = linked_args;
	while (*writer)
	{
		
	}
}

char	has_wildcards(char *str)
{
	char	in_single;
	char	in_double;

	in_single = 0;
	in_double = 0;
	while (*str)
	{
		if (!in_single && !in_double && *str == '\'')
			in_single = 1;
		else if (!in_single && !in_double && *str == '"')
			in_double = 1;
		else if (in_single && *str == '\'')
			in_single = 0;
		else if (in_double && *str == '"')
			in_double = 0;
		else if (!in_single && !in_double && *str == '*')
			return (1);
		str++;
	}
	return (0);
}

t_list	*get_files_list()
{
	t_list			*files_list;
	t_list			*file_node;
	char			*file_str;
	DIR				*dir;
	struct dirent	*entry;

	files_list = NULL;
	dir = opendir(".");
	if (!dir)
		return 0;
	entry = readdir(dir);
	while (entry) // Read and print each entry in the directory
	{
		if (*entry->d_name != '.')
		{
			file_str = ft_strdup(entry->d_name);
			if (!file_str)
				return (0);
			file_node = ft_lstnew(file_str);
			if (!file_node)
				return (free(file_str), NULL);
			ft_lstadd_back(&files_list, file_node);
		}
		entry = readdir(dir);
	}
	closedir(dir);	// Close the directory
	return (files_list);
}

char	hits_wildcard(char *wc, char *str, char in_sgl, char in_dbl)
{
	if (!*wc && !*str)
		return (1);
	else if (!*wc)
		return (0);
	if (!in_sgl && !in_dbl && *wc == '\'')
		return (hits_wildcard(wc + 1, str, 1, 0));
	else if (!in_sgl && !in_dbl && *wc == '"')
		return (hits_wildcard(wc + 1, str, 0, 1));
	else if ((in_sgl && *wc == '\'') || (in_dbl && *wc == '"'))
		return (hits_wildcard(wc + 1, str, 0, 0));
	if (!in_sgl && !in_dbl && *wc == '*')
	{
		if (!*str || *(wc + 1) == '*')
			return (hits_wildcard(wc + 1, str, 0, 0));
		else if (!*(wc + 1))
			return (1);
		else if (*(wc + 1) != *str)
			return (hits_wildcard(wc, str + 1, 0, 0));
		else if (*(wc + 1) == *str)
			return (hits_wildcard(wc + 1, str, 0, 0) || hits_wildcard(wc, str + 1, 0, 0));
	}
	if (*wc == *str)
		return (hits_wildcard(wc + 1, str + 1, in_sgl, in_dbl));
	return (0);
}

// char	hits_wildcard(char *wildcard, char *str)
// {
// 	if (!*wildcard && !*str)
// 		return (1);
// 	else if (!*wildcard)
// 		return (0);
// 	if (*wildcard == '*')
// 	{
// 		if (!*str)
// 			return (hits_wildcard(wildcard + 1, str));
// 		else if (!*(wildcard + 1))
// 			return (1);
// 		else if (*(wildcard + 1) == '*')
// 			return (hits_wildcard(wildcard + 1, str));
// 		else if (*(wildcard + 1) != *str)
// 			return (hits_wildcard(wildcard, str + 1));
// 		else if (*(wildcard + 1) == *str)
// 			return (hits_wildcard(wildcard + 1, str) || hits_wildcard(wildcard, str + 1));
// 	}
// 	if (*wildcard == *str)
// 		return (hits_wildcard(wildcard + 1, str + 1));
// 	return (0);
// }


void	clean_quotations(char *str)
{
	char	*writer;
	char	in_single;
	char	in_double;
	
	in_single = 0;
	in_double = 0;
	writer = str;
	while (*str)
	{
		if (in_single || in_double)
		{
			if ((in_single && *str == '\'') || (in_double && *str == '"'))
			{
				in_single = 0;
				in_double = 0;
				str++;
				continue ;
			}
		}
		else if (*str == '\'' || *str == '"')
		{
			if (*str == '\'')
				in_single = 1;
			else if (*str == '"')
				in_double = 1;
			str++;
			continue ;
		}
		*(writer++) = *(str++);
	}
	*writer = '\0';
}

// CLEANING OUTPUT FROM QUOTATIONS:
// compile with:  cc src/wildcard.c -L./src/Libft_extended -lft
// int	main(int argc, char **argv)
// {
// 	if (argc == 2)
// 	{
// 		clean_quotations(argv[1]);
// 		ft_printf("%s", argv[1]);
// 	}
// 	return (0);
// }

// compile with:  cc src/wildcard.c -L./src/Libft_extended -lft
// int main(int argc, char **argv)
// {
// 	t_list	*files_list;
// 	t_list	*searcher;
// 	t_list	**writer;

// 	if (argc != 2)
// 	{
// 		ft_printf("Use a wildcard-string as an argument!\n");
// 		return (EXIT_FAILURE);
// 	}
// 	files_list = get_files_list();
// 	ft_printf("There are %i files in this directory:\n", ft_lstsize(files_list));
// 	searcher = files_list;
// 	while (searcher)
// 	{
// 		ft_printf("name: %s\n", searcher->content);
// 		searcher = searcher->next;
// 	}
// 	ft_printf("\nYour given input is: %s\n", argv[1]);
	
// 	t_list	*tmp;
// 	writer = &files_list;
// 	while (*writer)
// 	{
// 		if (hits_wildcard(argv[1], (*writer)->content))
// 			writer = &(*writer)->next;
// 		else
// 		{
// 			ft_printf("Removing: %s\n", (*writer)->content);
// 			tmp = (*writer)->next;
// 			free((*writer)->content);
// 			free(*writer);
// 			*writer = tmp;
// 		}
// 	}
// 	ft_printf("\nThere are %i files matching the input:\n", ft_lstsize(files_list));
// 	searcher = files_list;
// 	while (searcher)
// 	{
// 		ft_printf("name: %s\n", searcher->content);
// 		searcher = searcher->next;
// 	}
//     return (0);
// }

/*
WILDCARD:
in the directory:
header  _infos  Makefile  minishell  obj  README.md  src
" ls * "
expands to
" ls header _infos/ Makefile minishell obj README.md src "

Idea to recreate wildcard: 
1. Use opendir and readdir to open and read the directory specified by the token.
2. Match the wildcard pattern with the entries in the directory.
3. Append matching filenames to the arg list.
*/
