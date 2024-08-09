/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wildcard.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bschneid <bschneid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/02 15:16:24 by bschneid          #+#    #+#             */
/*   Updated: 2024/08/09 18:56:56 by bschneid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minishell.h"

// * (asterisk) this can represent any number of characters (including zero, 
	// in other words, zero or more characters). If you specified a "cd*" it 
	// would use "cda", "cdrom", "cdrecord" and anything that starts with “cd”
	// also including “cd” itself. "m*l" could by mill, mull, ml, and anything 
	// that starts with an m and ends with an l.

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

char	hits_wildcard(char *wildcard, char *str)
{
	if (!*wildcard && !*str)
		return (1);
	else if (!*wildcard)
		return (0);
	if (*wildcard == '*')
	{
		if (!*str)
			return (hits_wildcard(wildcard + 1, str));
		else if (!*(wildcard + 1))
			return (1);
		else if (*(wildcard + 1) == '*')
			return (hits_wildcard(wildcard + 1, str));
		else if (*(wildcard + 1) != *str)
			return (hits_wildcard(wildcard, str + 1));
		else if (*(wildcard + 1) == *str)
			return (hits_wildcard(wildcard + 1, str) || hits_wildcard(wildcard, str + 1));
	}
	if (*wildcard == *str)
		return (hits_wildcard(wildcard + 1, str + 1));
	return (0);
}

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
